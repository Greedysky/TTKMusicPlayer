#include "echoplugin.h"
#include <QSettings>

static int chsign(int x, int s)
{
    return (x < 0) ^ (s < 0) ? -x : x;
}

static int rdiv(int x, int y)
{
    return (x + chsign(y / 2, x)) / y;
}

static int rescale(int x, int old_scale, int new_scale)
{
    return rdiv(x * new_scale, old_scale);
}

EchoPlugin *EchoPlugin::m_instance = nullptr;

EchoPlugin::EchoPlugin()
    : Effect()
{
    m_instance = this;

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_delay = settings.value("Echo/delay", 500).toUInt();
    m_feedback = settings.value("Echo/feedback", 50).toUInt();
    m_volume = settings.value("Echo/volume", 50).toUInt();
}

EchoPlugin::~EchoPlugin()
{
    m_instance = nullptr;
    if(m_buffer)
    {
        delete[] m_buffer;
    }
}

void EchoPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    const float feedback = m_feedback / 100.0f;
    const float volume = m_volume / 100.0f;
    int interval = rescale(m_delay, 1000, m_freq) * m_chan;
    interval = std::min(std::max(interval, 0), m_size);  // sanity check

    int offset = m_offset - interval;
    if(offset < 0)
    {
        offset += m_size;
    }

    float *data = b->data;
    for(uint i = 0; i < b->samples; i ++)
    {
        float in = data[i];
        float buf = m_buffer[offset];

        data[i] = in + buf * volume;
        m_buffer[m_offset] = in + buf * feedback;

        offset = (offset + 1) % m_size;
        m_offset = (m_offset + 1) % m_size;
    }
    m_mutex.unlock();
}

void EchoPlugin::configure(quint32 freq, ChannelMap map)
{
    if(m_chan != map.count() || m_freq != freq)
    {
        m_chan = map.count();
        m_freq = freq;
        m_offset = 0;

        if(m_buffer)
        {
            delete[] m_buffer;
        }

        m_size = rescale(1000, 1000, m_freq) * m_chan;
        m_buffer = new float[m_size]{0};
    }

    Effect::configure(freq, map);
}

void EchoPlugin::setDelay(int delay)
{
    m_mutex.lock();
    m_delay = delay;
    m_mutex.unlock();
}

void EchoPlugin::setFeedback(int feedback)
{
    m_mutex.lock();
    m_feedback = feedback;
    m_mutex.unlock();
}

void EchoPlugin::setVolume(int volume)
{
    m_mutex.lock();
    m_volume = volume;
    m_mutex.unlock();
}

EchoPlugin* EchoPlugin::instance()
{
    return m_instance;
}
