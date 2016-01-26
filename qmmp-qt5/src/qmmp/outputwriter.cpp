/***************************************************************************
 *   Copyright (C) 2012-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <string.h>
#include "statehandler.h"
#include "visual.h"
#include "output.h"
#include "audioconverter_p.h"
#include "channelconverter_p.h"
#include "volumecontrol_p.h"
#include "outputwriter_p.h"

extern "C" {
#include "equ/iir.h"
}

//static functions
static inline void s8_to_s16(qint8 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] << 8;
    return;
}

static inline void s24_to_s16(qint32 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] >> 8;
    return;
}

static inline void s32_to_s16(qint32 *in, qint16 *out, qint64 samples)
{
    for(qint64 i = 0; i < samples; ++i)
        out[i] = in[i] >> 16;
    return;
}

OutputWriter::OutputWriter (QObject* parent) : QThread (parent)
{
    m_handler = StateHandler::instance();
    m_frequency = 0;
    m_channels = 0;
    m_output = 0;
    m_format = Qmmp::PCM_UNKNOWM;
    m_totalWritten = 0;
    m_currentMilliseconds = -1;
    m_bytesPerMillisecond = 0;
    m_user_stop = false;
    m_finish = false;
    m_visBuffer = 0;
    m_visBufferSize = 0;
    m_kbps = 0;
    m_skip = false;
    m_pause = false;
    m_prev_pause = false;
    m_useEq = false;
    m_muted = false;
    m_settings = QmmpSettings::instance();
}

OutputWriter::~OutputWriter()
{
    if(m_output)
    {
        delete m_output;
        m_output = 0;
    }
    if(m_visBuffer)
    {
        delete[] m_visBuffer;
        m_visBuffer = 0;
    }
}

bool OutputWriter::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    QMap<Qmmp::AudioFormat, QString> formatNames;
    formatNames.insert(Qmmp::PCM_S8, "s8");
    formatNames.insert(Qmmp::PCM_S16LE, "s16le");
    formatNames.insert(Qmmp::PCM_S24LE, "s24le");
    formatNames.insert(Qmmp::PCM_S32LE, "s32le");

    m_output = Output::create();
    if(!m_output)
    {
        qWarning("OutputWriter: unable to create output");
        return false;
    }

    if (!m_output->initialize(freq, map, format))
    {
        qWarning("OutputWriter: unable to initialize output");
        delete m_output;
        m_output = 0;
        return false;
    }
    m_frequency = freq;
    m_channels = map.count();
    m_format = format;
    m_chan_map = map;

    qDebug("OutputWriter: [%s] %u Hz, {%s}, %s ==> %u Hz, {%s}, %s",
           qPrintable(Output::currentFactory()->properties().shortName),
           freq,
           qPrintable(map.toString()),
           qPrintable(formatNames.value(format)),
           m_output->sampleRate(),
           qPrintable(m_output->channelMap().toString()),
           qPrintable(formatNames.value(m_output->format())));

    if(!prepareConverters())
    {
        qWarning("OutputWriter: unable to convert audio");
        delete m_output;
        m_output = 0;
        return false;
    }

    m_bytesPerMillisecond = m_frequency * m_channels * AudioParameters::sampleSize(format) / 1000;
    m_recycler.configure(m_frequency, m_channels, m_format); //calculate output buffer size
    //visual buffer
    if(m_visBuffer)
        delete [] m_visBuffer;
    m_visBufferSize = QMMP_BLOCK_FRAMES * 2 * m_channels; //16-bit samples
    if(m_format != Qmmp::PCM_S16LE)
        m_visBuffer = new unsigned char [m_visBufferSize];
    updateEqSettings();
    clean_history();
    return true;
}

void OutputWriter::pause()
{
    mutex()->lock();
    m_pause = !m_pause;
    mutex()->unlock();
    Qmmp::State state = m_pause ? Qmmp::Paused: Qmmp::Playing;
    dispatch(state);
}

void OutputWriter::stop()
{
    m_user_stop = true;
}

void OutputWriter::setMuted(bool muted)
{
    m_muted = muted;
}

void OutputWriter::finish()
{
    m_finish = true;
}

void OutputWriter::seek(qint64 pos, bool reset)
{
    m_totalWritten = pos * m_bytesPerMillisecond;
    m_currentMilliseconds = -1;
    m_skip = isRunning() && reset;
}

Recycler *OutputWriter::recycler()
{
    return &m_recycler;
}

QMutex *OutputWriter::mutex()
{
    return &m_mutex;
}

AudioParameters OutputWriter::audioParameters() const
{
    return AudioParameters(m_frequency, m_chan_map, m_format);
}

quint32 OutputWriter::sampleRate()
{
    return m_frequency;
}

int OutputWriter::channels()
{
    return m_channels;
}

Qmmp::AudioFormat OutputWriter::format() const
{
    return m_format;
}

const ChannelMap OutputWriter::channelMap() const
{
    return m_chan_map;
}

int OutputWriter::sampleSize() const
{
    return AudioParameters::sampleSize(m_format);
}

void OutputWriter::dispatchVisual (Buffer *buffer)
{
    if (!buffer)
        return;

    int sampleSize = AudioParameters::sampleSize(m_format);
    int samples = buffer->nbytes/sampleSize;
    int outSize = samples*2;
    if((m_format != Qmmp::PCM_S16LE) && outSize > m_visBufferSize) //increase buffer size
    {
        delete[] m_visBuffer;
        m_visBufferSize = outSize;
        m_visBuffer = new unsigned char [m_visBufferSize];
    }
    switch(m_format)
    {
    case Qmmp::PCM_S8:
        s8_to_s16((qint8 *)buffer->data, (qint16 *) m_visBuffer, samples);
        break;
    case Qmmp::PCM_S16LE:
        m_visBuffer = buffer->data;
        outSize = buffer->nbytes;
        break;
    case Qmmp::PCM_S24LE:
        s24_to_s16((qint32 *)buffer->data, (qint16 *) m_visBuffer, samples);
        break;
    case Qmmp::PCM_S32LE:
        s32_to_s16((qint32 *)buffer->data, (qint16 *) m_visBuffer, samples);
        break;
    default:
        return;
    }
    foreach (Visual *visual, *Visual::visuals())
    {
        visual->mutex()->lock ();
        visual->add (m_visBuffer, outSize, m_channels);
        visual->mutex()->unlock();
    }
    if(m_format == Qmmp::PCM_S16LE)
        m_visBuffer = 0;
}

void OutputWriter::applyConverters(Buffer *buffer)
{
    for (int i = 0; i < m_converters.count(); ++i)
    {
        m_converters[i]->applyEffect(buffer);
    }
}

void OutputWriter::clearVisuals()
{
    foreach (Visual *visual, *Visual::visuals())
    {
        visual->mutex()->lock ();
        visual->clear ();
        visual->mutex()->unlock();
    }
}

bool OutputWriter::prepareConverters()
{
    qDeleteAll(m_converters);
    m_converters.clear();

    AudioParameters ap = m_output->audioParameters();

    if(channels() != m_output->channels())
    {
        qWarning("OutputWriter: unsupported channel number");
        return false;
    }

    if(format() != ap.format())
    {
        if(m_output->format() == Qmmp::PCM_S16LE)
        {
            qDebug("OutputWriter: using 16 bit comverter");
            m_converters << new AudioConverter();
            m_converters.last()->configure(sampleRate(), channelMap(), format());
        }
        else
        {
            qWarning("OutputWriter: unsupported audio format");
            return false;
        }
    }

    if(channelMap() != ap.channelMap())
    {
        m_converters << new ChannelConverter(ap.channelMap());
        m_converters.last()->configure(sampleRate(), channelMap(), ap.format());
    }

    return true;
}

void OutputWriter::dispatch(qint64 elapsed,
                      int bitrate,
                      int frequency,
                      int precision,
                      int channels)
{
    if (m_handler)
        m_handler->dispatch(elapsed, bitrate, frequency, precision, channels);
}

void OutputWriter::dispatch(const Qmmp::State &state)
{
    if (m_handler)
        m_handler->dispatch(state);
    if (state == Qmmp::Stopped)
        clearVisuals();
}

void OutputWriter::run()
{
    mutex()->lock ();
    if (!m_bytesPerMillisecond)
    {
        qWarning("OutputWriter: invalid audio parameters");
        mutex()->unlock ();
        return;
    }
    mutex()->unlock ();

    bool done = false;
    Buffer *b = 0;
    quint64 l;
    qint64 m = 0;

    dispatch(Qmmp::Playing);

    while (!done)
    {
        mutex()->lock ();
        if(m_pause != m_prev_pause)
        {
            if(m_pause)
            {
                m_output->suspend();
                mutex()->unlock();
                m_prev_pause = m_pause;
                continue;
            }
            else
                m_output->resume();
            m_prev_pause = m_pause;
        }
        recycler()->mutex()->lock ();
        done = m_user_stop || (m_finish && recycler()->empty());

        while (!done && (recycler()->empty() || m_pause))
        {
            recycler()->cond()->wakeOne();
            mutex()->unlock();
            recycler()->cond()->wait(recycler()->mutex());
            mutex()->lock ();
            done = m_user_stop || m_finish;
        }

        status();
        if (!b)
        {
            b = recycler()->next();
            if (b && b->rate)
                m_kbps = b->rate;
        }

        recycler()->cond()->wakeOne();
        recycler()->mutex()->unlock();
        mutex()->unlock();
        if (b)
        {
            mutex()->lock();
            if (m_useEq)
            {
                switch(m_format)
                {
                case Qmmp::PCM_S16LE:
                    iir((void*) b->data, b->nbytes, m_channels);
                    break;
                case Qmmp::PCM_S24LE:
                    iir24((void*) b->data, b->nbytes, m_channels);
                    break;
                case Qmmp::PCM_S32LE:
                    iir32((void*) b->data, b->nbytes, m_channels);
                    break;
                default:
                    ;
                }
            }
            mutex()->unlock();
            dispatchVisual(b);
            if (SoftwareVolume::instance())
                SoftwareVolume::instance()->changeVolume(b, m_channels, m_format);
            if (m_muted)
                memset(b->data, 0, b->nbytes);
            applyConverters(b);
            l = 0;
            m = 0;
            while (l < b->nbytes && !m_pause && !m_prev_pause)
            {
                mutex()->lock();
                if(m_skip)
                {
                    m_skip = false;
                    m_output->reset();
                    mutex()->unlock();
                    break;
                }
                mutex()->unlock();
                m = m_output->writeAudio(b->data + l, b->nbytes - l);
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
        mutex()->lock();
        //force buffer change
        recycler()->mutex()->lock ();
        recycler()->done();
        recycler()->mutex()->unlock();
        b = 0;
        mutex()->unlock();
    }
    mutex()->lock ();
    //write remaining data
    if(m_finish)
    {
        m_output->drain();
#ifdef Q_OS_WIN
        qDebug("OutputWriter: total written %I64d", m_totalWritten);
#else
        qDebug("OutputWriter: total written %lld", m_totalWritten);
#endif
    }
    dispatch(Qmmp::Stopped);
    mutex()->unlock();
}

void OutputWriter::status()
{
    qint64 ct = m_totalWritten / m_bytesPerMillisecond - m_output->latency();

    if (ct < 0)
        ct = 0;

    if (ct > m_currentMilliseconds)
    {
        m_currentMilliseconds = ct;
        dispatch(m_currentMilliseconds, m_kbps,
                 m_frequency, AudioParameters::sampleSize(m_format)*8, m_channels);
    }
}

void OutputWriter::updateEqSettings()
{
    mutex()->lock();
    if(m_settings->eqSettings().isEnabled())
    {
        double preamp = m_settings->eqSettings().preamp();
        int bands =  m_settings->eqSettings().bands();

        init_iir(m_frequency, bands);

        set_preamp(0, 1.0 + 0.0932471 *preamp + 0.00279033 * preamp * preamp);
        set_preamp(1, 1.0 + 0.0932471 *preamp + 0.00279033 * preamp * preamp);
        for(int i = 0; i < bands; ++i)
        {
            double value =  m_settings->eqSettings().gain(i);
            set_gain(i,0, 0.03*value+0.000999999*value*value);
            set_gain(i,1, 0.03*value+0.000999999*value*value);
        }
    }
    m_useEq = m_settings->eqSettings().isEnabled();
    mutex()->unlock();
}
