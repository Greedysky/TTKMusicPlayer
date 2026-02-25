#include "statehandler.h"
#include "visual.h"
#include "output.h"
#include "audioconverter.h"
#include "channelconverter_p.h"
#include "volumehandler.h"
#include "outputwriter_p.h"

extern "C" {
#include "equ/iir.h"
}

OutputWriter::OutputWriter(QObject* parent)
    : QThread(parent),
      m_handler(StateHandler::instance()),
      m_settings(QmmpSettings::instance())
{

}

OutputWriter::~OutputWriter()
{
    delete m_output;
    delete m_format_converter;
    delete m_channel_converter;
    delete[] m_output_buf;
}

bool OutputWriter::initialize(quint32 freq, ChannelMap map)
{
    m_in_params = AudioParameters(freq, map, Qmmp::PCM_FLOAT);
    m_output = Output::create();
    if(!m_output)
    {
        qWarning("OutputWriter: unable to create output");
        return false;
    }

    if(!m_output->initialize(freq, map, m_settings->outputFormat()))
    {
        qWarning("OutputWriter: unable to initialize output");
        delete m_output;
        m_output = nullptr;
        return false;
    }

    m_frequency = m_output->sampleRate();
    m_chan_map = m_output->channelMap();
    m_channels = m_chan_map.count();
    m_format = m_output->format();
    m_abr = m_settings->averageBitrate();

    qDebug("OutputWriter: [%s] %s ==> %s",
           qPrintable(Output::currentFactory()->properties().shortName),
           qPrintable(m_in_params.toString()), qPrintable(m_output->audioParameters().toString()));

    if(!prepareConverters())
    {
        qWarning("OutputWriter: unable to convert audio");
        delete m_output;
        m_output = nullptr;
        return false;
    }

    delete[] m_output_buf;
    m_output_size = QMMP_BLOCK_FRAMES * m_channels * 4;
    m_output_buf = new unsigned char[m_output_size * m_output->sampleSize()];

    m_bytesPerMillisecond = m_frequency * m_channels * AudioParameters::sampleSize(m_format) / 1000;
    m_recycler.configure(m_in_params.sampleRate(), m_in_params.channels()); //calculate output buffer size
    updateEqSettings();
    eq_clean_history();
    return true;
}

void OutputWriter::pause()
{
    m_pause = !m_pause;
    Qmmp::State state = m_pause ? Qmmp::Paused : Qmmp::Playing;
    dispatch(state);
}

void OutputWriter::stop()
{
    m_user_stop = true;
}

void OutputWriter::finish()
{
    m_finish = true;
}

void OutputWriter::seek(qint64 time, bool reset)
{
    m_mutex.lock();
    m_totalWritten = time * m_bytesPerMillisecond;
    m_currentMilliseconds = -1;
    m_skip = isRunning() && reset;
    m_mutex.unlock();
}

Recycler *OutputWriter::recycler()
{
    return &m_recycler;
}

const AudioParameters &OutputWriter::inputAudioParameters() const
{
    return m_in_params;
}

AudioParameters OutputWriter::outputAudioParameters() const
{
    return AudioParameters(m_frequency, m_chan_map, m_format);
}

int OutputWriter::sampleSize() const
{
    return AudioParameters::sampleSize(m_format);
}

void OutputWriter::dispatchVisual(Buffer *buffer)
{
    if(!buffer)
        return;

    Visual::addAudio(buffer->data, buffer->samples, m_channels,
                     m_totalWritten / m_bytesPerMillisecond, m_output->latency());
}

bool OutputWriter::prepareConverters()
{
    delete m_format_converter;
    m_format_converter = nullptr;

    delete m_channel_converter;
    m_channel_converter = nullptr;

    if(m_channels != m_output->channels())
    {
        qWarning("OutputWriter: unsupported channel number");
        return false;
    }

    if(m_in_params.format() != m_format)
    {
        m_format_converter = new AudioConverter();
        m_format_converter->configure(m_format);
    }

    if(m_in_params.channelMap() != m_chan_map)
    {
        m_channel_converter = new ChannelConverter(m_chan_map);
        m_channel_converter->configure(m_in_params.sampleRate(), m_in_params.channelMap());
    }
    return true;
}

void OutputWriter::startVisualization()
{
    for(Visual *visual : *Visual::visuals())
    {
        QMetaObject::invokeMethod(visual, "start", Qt::QueuedConnection);
    }
}

void OutputWriter::stopVisualization()
{
    Visual::clearBuffer();
    for(Visual *visual : *Visual::visuals())
    {
        QMetaObject::invokeMethod(visual, "stop", Qt::QueuedConnection);
    }
}

void OutputWriter::dispatch(qint64 elapsed, int bitrate)
{
    if(m_handler)
        m_handler->dispatch(elapsed, bitrate);
}

void OutputWriter::dispatch(const Qmmp::State &state)
{
    if(m_handler)
        m_handler->dispatch(state);
}

void OutputWriter::dispatch(const AudioParameters &p)
{
    if(m_handler)
        m_handler->dispatch(p);
}

void OutputWriter::run()
{
    m_mutex.lock();
    if(!m_bytesPerMillisecond)
    {
        qWarning("OutputWriter: invalid audio parameters");
        m_mutex.unlock();
        return;
    }
    m_mutex.unlock();

    bool done = false;
    Buffer *b = nullptr;
    quint64 l;
    qint64 m = 0;
    size_t output_at = 0;
    unsigned char *tmp = nullptr;

    dispatch(Qmmp::Playing);
    dispatch(m_output->audioParameters());
    startVisualization();

    while(!done)
    {
        m_mutex.lock();
        if(m_pause != m_paused)
        {
            m_paused = m_pause;
            if(m_paused)
            {
                Visual::clearBuffer();
                m_output->suspend();
                m_mutex.unlock();
                continue;
            }
            
            m_output->resume();
        }
        recycler()->mutex()->lock();
        done = m_user_stop || (m_finish && recycler()->empty());

        while(!done && (recycler()->empty() || m_pause))
        {
            recycler()->cond()->wakeOne();
            m_mutex.unlock();
            recycler()->cond()->wait(recycler()->mutex());
            m_mutex.lock();
            done = m_user_stop || m_finish;
        }

        status();
        if(!b)
        {
            if((b = recycler()->next()))
            {
                if(b->rate && !m_abr)
                    m_kbps = b->rate;
                if(b->trackInfo)
                {
                    m_output->setTrackInfo(*b->trackInfo);
                    if(m_abr)
                        m_kbps = b->trackInfo->value(Qmmp::BITRATE).toInt();
                }
            }
        }

        recycler()->cond()->wakeOne();
        recycler()->mutex()->unlock();
        m_mutex.unlock();
        if(b)
        {
            m_mutex.lock();
            if(m_useEq)
            {
                eq_iir(b->data, b->samples, m_channels);
            }
            m_mutex.unlock();
            dispatchVisual(b);
            if(VolumeHandler::instance())
                VolumeHandler::instance()->apply(b, m_channels);
            if(m_channel_converter)
                m_channel_converter->applyEffect(b);
            l = 0;
            m = 0;

            //increase buffer size if needed
            if(b->samples > m_output_size)
            {
                delete[] m_output_buf;
                m_output_size = b->samples;
                m_output_buf = new unsigned char[m_output_size * sampleSize()];
            }

            if(m_format_converter)
            {
                m_format_converter->fromFloat(b->data, m_output_buf, b->samples);
                tmp = m_output_buf;
            }
            else
            {
                tmp = (unsigned char*)b->data;
            }
            output_at = b->samples * m_output->sampleSize();

            while(l < output_at && !m_pause && !m_paused)
            {
                m_mutex.lock();
                if(m_skip)
                {
                    m_skip = false;
                    Visual::clearBuffer();
                    m_output->reset();
                    m_mutex.unlock();
                    break;
                }
                m_mutex.unlock();
                m = m_output->writeAudio(tmp + l, output_at - l);
                if(m >= 0)
                {
                    m_totalWritten += m;
                    l+= m;
                }
                else
                    break;
            }
            if(m < 0)
                break;
        }
        //force buffer change
        recycler()->mutex()->lock();
        recycler()->done();
        recycler()->mutex()->unlock();
        b = nullptr;
    }
    m_mutex.lock();
    //write remaining data
    if(m_finish)
    {
        m_output->drain();
        //qDebug() << "OutputWriter: total written" << m_totalWritten;
    }
    dispatch(Qmmp::Stopped);
    stopVisualization();
    m_mutex.unlock();
}

void OutputWriter::status()
{
    qint64 ct = m_totalWritten / m_bytesPerMillisecond - m_output->latency();

    if(ct < 0)
        ct = 0;

    if(m_currentMilliseconds == -1) //refresh audio properties in the begin of track
        dispatch(m_output->audioParameters());

    if(ct > m_currentMilliseconds)
    {
        m_currentMilliseconds = ct;
        dispatch(m_currentMilliseconds, m_kbps);
    }
}

void OutputWriter::updateEqSettings()
{
    m_mutex.lock();
    if(m_settings->eqSettings().isEnabled())
    {
        double preamp = m_settings->eqSettings().preamp();
        int bands = m_settings->eqSettings().bands();

        eq_init_iir(m_frequency, bands);
        eq_set_option(EQ_TWO_PASSES, m_settings->eqSettings().twoPasses());
        eq_set_option(EQ_CLIP, 1);

        for(int ch = 0; ch < m_channels; ++ch)
        {
            eq_set_preamp(ch, 1.0 + 0.0932471 *preamp + 0.00279033 * preamp * preamp);

            for(int i = 0; i < bands; ++i)
            {
                double value = m_settings->eqSettings().gain(i);
                eq_set_gain(i, ch, 0.03 * value + 0.000999999 * value * value);
            }
        }
    }
    m_useEq = m_settings->eqSettings().isEnabled();
    m_mutex.unlock();
}
