#include "crossfadeplugin.h"

#include <QSettings>
#include <qmmp/statehandler.h>
#include <qmmp/soundcore.h>

CrossfadePlugin::CrossfadePlugin()
    : Effect(),
      m_core(SoundCore::instance()),
      m_handler(StateHandler::instance())
{
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_overlap = settings.value("Crossfade/overlap", 6000).toLongLong();
}

CrossfadePlugin::~CrossfadePlugin()
{
    if(m_buffer)
    {
        free(m_buffer);
    }
}

void CrossfadePlugin::applyEffect(Buffer *b)
{
    switch(m_state)
    {
    case Waiting:
        if((m_core->duration() > m_overlap + 2000) && (m_core->duration() - m_handler->elapsed() < m_overlap + 2000))
        {
            StateHandler::instance()->sendNextTrackRequest();
            m_state = Checking;
        }
        break;
    case Checking:
        //next source has been received and current engine will be used to play it
        if(SoundCore::instance()->nextTrackAccepted())
        {
            m_state = Preparing;
        }
        break;
    case Preparing:
        if(m_core->duration() && (m_core->duration() - m_handler->elapsed() <  m_overlap))
        {
            if(m_bufferAt + b->samples > m_bufferSize)
            {
                m_bufferSize = m_bufferAt + b->samples;
                float *buffer = m_buffer;
                m_buffer = (float *)realloc(m_buffer, m_bufferSize * sizeof(float));
                if(!m_buffer)
                {
                    qWarning("CrossfadePlugin: unable to allocate  %zu bytes", m_bufferSize);
                    m_bufferSize = 0;
                    if(buffer)
                    {
                        free(buffer);
                    }
                }
            }

            if(m_buffer)
            {
                memcpy(m_buffer + m_bufferAt, b->data, b->samples * sizeof(float));
                m_bufferAt += b->samples;
                b->samples = 0;
            }
        }
        else if(m_bufferAt > 0)
        {
            m_state = Processing;
        }
        break;
    case Processing:
        if(m_bufferAt > 0)
        {
            const double volume = (double)m_bufferAt/m_bufferSize;
            const size_t samples = qMin(m_bufferAt, b->samples);
            mix(b->data, m_buffer, samples, volume);
            m_bufferAt -= samples;
            memmove(m_buffer, m_buffer + samples, m_bufferAt * sizeof(float));
        }
        else
        {
            m_state = Waiting;
        }
        break;
    default:
        break;
    }
    return;
}

void CrossfadePlugin::mix(float *cur_buf, float *prev_buf, uint samples, double volume)
{
    for(uint i = 0; i < samples; ++i)
    {
        cur_buf[i] = cur_buf[i] * (1.0 - volume) + prev_buf[i] * volume;
        cur_buf[i] = qBound(-1.0f, cur_buf[i], 1.0f);
    }
}
