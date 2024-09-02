#include <math.h>
#include <qmmp/inputsourcefactory.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/audioconverter.h>
#include "replaygainner.h"

#define BUFFER_FRAMES 4096

ReplayGainner::ReplayGainner()
{

}

ReplayGainner::~ReplayGainner()
{
    stop();
    deinit();
    if(m_handle)
    {
        DeinitGainAnalysis(m_handle);
        m_handle = nullptr;
    }
}

bool ReplayGainner::prepare(const QString &url)
{
    deinit();

    m_url = url;
    QString name = m_url.section("/", -1);
    InputSource *source = InputSource::create(url, nullptr);
    if(!source->initialize())
    {
        delete source;
        qWarning("ReplayGainner: Invalid url");
        return false;
    }

    if(source->ioDevice() && !source->ioDevice()->open(QIODevice::ReadOnly))
    {
        qWarning("ReplayGainner: [%s] unable to open input stream, error: %s",
                 qPrintable(name),
                 qPrintable(source->ioDevice()->errorString()));
        delete source;
        return false;
    }

    DecoderFactory *factory = Decoder::findByFilePath(source->path());

    if(!factory)
    {
        qWarning("ReplayGainner: [%s] unable to find factory", qPrintable(name));
        delete source;
        return false;
    }
    qDebug("ReplayGainner: [%s] selected decoder: %s",qPrintable(name),
           qPrintable(factory->properties().shortName));

    if(factory->properties().noInput && source->ioDevice())
    {
        source->ioDevice()->close();
    }

    Decoder *decoder = factory->create(source->path(), source->ioDevice());
    if(!decoder->initialize())
    {
        qWarning("ReplayGainner: [%s] invalid file format", qPrintable(name));
        delete source;
        delete decoder;
        return false;
    }

    if(decoder->audioParameters().channels() > 2)
    {
        qWarning("ReplayGainner: [%s] unsupported channel number: %d",
                 qPrintable(name),
                 decoder->audioParameters().channels());
        delete source;
        delete decoder;
        return false;
    }

    m_decoder = decoder;
    m_source = source;
    m_stop = false;
    m_hasValues = false;
    return true;
}

void ReplayGainner::stop()
{
    m_mutex.lock();
    m_stop = true;
    m_mutex.unlock();
}

bool ReplayGainner::hasValues() const
{
    return m_hasValues;
}

QMap<Qmmp::ReplayGainKey, double> ReplayGainner::oldReplayGainInfo() const
{
    if(!m_decoder)
    {
        return QMap<Qmmp::ReplayGainKey, double>();
    }

    return m_decoder->replayGainInfo();
}

double ReplayGainner::gain() const
{
    return m_gain;
}

double ReplayGainner::peak() const
{
    return m_peak;
}

QString ReplayGainner::url() const
{
    return m_url;
}

GainHandle_t *ReplayGainner::handle()
{
    return m_handle;
}

void ReplayGainner::run()
{
    if(m_stop)
    {
        return;
    }

    QString name = m_url.section("/", -1);
    qDebug("ReplayGainner: [%s] staring thread", qPrintable(name));
    bool error = false;

    AudioParameters ap = m_decoder->audioParameters();
    AudioConverter converter;
    converter.configure(ap.format());
    //buffers
    double out_left[BUFFER_FRAMES] = {0}, out_right[BUFFER_FRAMES] = {0}; //replay gain buffers
    float *float_buf = new float[BUFFER_FRAMES * ap.channels()]; //float buffer
    qint64 buf_size = BUFFER_FRAMES * ap.frameSize();
    unsigned char *char_buf = new unsigned char[buf_size]; //char buffer

    //counters
    qint64 totalSamples = m_decoder->totalTime() * ap.sampleRate() * ap.channels() / 1000, len = 0;
    quint64 sample_counter = 0;
    quint64 samples = 0;
    double max = 0;

    if(m_handle)
    {
        DeinitGainAnalysis(m_handle);
    }
    InitGainAnalysis(&m_handle, ap.sampleRate());

    forever
    {
        len = m_decoder->read(char_buf, buf_size);

        if(len < 0 || totalSamples == 0)
        {
            error = true;
            break;
        }
        else if(len == 0)
        {
            break;
        }

        samples = len / ap.sampleSize();

        converter.toFloat(char_buf, float_buf, samples);

        if(ap.channels() == 2)
        {
            for(uint i = 0; i < (samples >> 1); ++i)
            {
                out_left[i] = float_buf[i*2]*32768.0;
                out_right[i] = float_buf[i*2+1]*32768.0;
                max = qMax(fabs(out_left[i]), max);
                max = qMax(fabs(out_right[i]), max);
            }
        }
        else if(ap.channels() == 1)
        {
            for(uint i = 0; i < samples; ++i)
            {
                out_left[i] = float_buf[i]*32768.0;
                max = qMax(fabs(out_left[i]), max);
            }
        }

        size_t samples_per_channel = samples >> ((ap.channels() == 2) ? 1 : 0);

        AnalyzeSamples(m_handle, out_left, out_right, samples_per_channel, ap.channels());
        sample_counter += samples;
        emit progress(100 * sample_counter / totalSamples);

        m_mutex.lock();
        if(m_stop)
        {
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();
    }

    delete[] float_buf;
    delete[] char_buf;

    if(error)
    {
        qWarning("ReplayGainner: [%s] finished with error", qPrintable(name));
    }
    else if(m_stop)
    {
        qDebug("ReplayGainner: [%s] stopped by user", qPrintable(name));
    }
    else
    {
        m_gain = GetTitleGain(m_handle);
        m_peak = max/32768.0;
        emit progress(100);
        qDebug("ReplayGainner: [%s] peak=%f, gain=%f", qPrintable(name), m_peak, m_gain);
        qDebug("ReplayGainner: [%s] finished with success ", qPrintable(name));
        m_hasValues = true;
    }

    deinit();
    emit finished(m_url);
}

void ReplayGainner::deinit()
{
    if(m_decoder)
    {
        delete m_decoder;
        m_decoder = nullptr;
    }

    if(m_source)
    {
        delete m_source;
        m_source = nullptr;
    }
}
