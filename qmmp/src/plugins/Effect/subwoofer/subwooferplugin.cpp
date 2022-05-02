#include "subwooferplugin.h"

#include <QSettings>

#define MAX_DELAY       100 /* msec */
#define MIN_CUTOFF      50 /* Hz */
#define MAX_SRATE       50000 /* Hz */
#define BUFFER_FRAMES   ((MAX_SRATE / 2 / MIN_CUTOFF + MAX_SRATE * MAX_DELAY / 1000) * 2)
#define BUFFER_BYTES    (BUFFER_FRAMES * sizeof(float))

SubwooferPlugin *SubwooferPlugin::m_instance = nullptr;

SubwooferPlugin::SubwooferPlugin()
    : Effect()
{
    m_instance = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_delay = settings.value("Echo/delay", 20).toUInt();
    m_feedback = settings.value("Echo/feedback", 60).toUInt();
    m_cutoff = settings.value("Echo/cutoff", 100).toUInt();
}

SubwooferPlugin::~SubwooferPlugin()
{
    delete m_buffer;
}

void SubwooferPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    m_mutex.lock();

    int offset = 0;
    float output = 0.0f;
    const float *end = b->data + b->samples;
    const int count = sampleRate() / 2 / m_cutoff;

    for(float *data = b->data; data < end; ++data)
    {
        offset = m_offset - (sampleRate() * m_delay / 1000) * channels() - 1;
        output = 0.0f;

        for(int i = 0; i < count; ++i)
        {
            if(offset < 0)
            {
                offset += BUFFER_FRAMES;
            }

            output += m_buffer[offset];
            offset -= channels();
        }

        output = output / count;
        m_buffer[m_offset] = *data + output * (m_feedback / 100.0);
        *data = *data + output;

        if(++m_offset >= BUFFER_FRAMES)
        {
            m_offset -= BUFFER_FRAMES;
        }
    }
    m_mutex.unlock();
}

void SubwooferPlugin::configure(quint32 freq, ChannelMap map)
{
    if(channels() != map.count() || sampleRate() != freq)
    {
        Effect::configure(freq, map);
        if(!m_buffer)
        {
            m_buffer = new float[BUFFER_BYTES]{0};
        }

        memset(m_buffer, 0, BUFFER_BYTES);
        m_offset = 0;
    }
}

void SubwooferPlugin::setDelay(int delay)
{
    m_mutex.lock();
    m_delay = delay;
    m_mutex.unlock();
}

void SubwooferPlugin::setFeedback(int feedback)
{
    m_mutex.lock();
    m_feedback = feedback;
    m_mutex.unlock();
}

void SubwooferPlugin::setCutOff(int value)
{
    m_mutex.lock();
    m_cutoff = value;
    m_mutex.unlock();
}

SubwooferPlugin* SubwooferPlugin::instance()
{
    return m_instance;
}
