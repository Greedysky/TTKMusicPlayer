#include "echoplugin.h"

#include <QSettings>

#define CH_SIGN(x, y) ((x < 0) ^ (y < 0) ? -x : x)

static int rescale(int x, int old_scale, int new_scale)
{
    int v = x * new_scale;
    return (v + CH_SIGN(old_scale / 2, v)) / old_scale;
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
    delete[] m_buffer;
}

void EchoPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    const float feedback = m_feedback / 100.0f;
    const float volume = m_volume / 100.0f;
    int interval = rescale(m_delay, 1000, sampleRate()) * channels();
    interval = std::min(std::max(interval, 0), m_size);  // sanity check

    int offset = m_offset - interval;
    if(offset < 0)
    {
        offset += m_size;
    }

    float *data = b->data;
    for(size_t i = 0; i < b->samples; ++i)
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
    if(channels() != map.count() || sampleRate() != freq)
    {
        Effect::configure(freq, map);

        delete[] m_buffer;
        m_size = rescale(1000, 1000, sampleRate()) * channels();
        m_buffer = new float[m_size]{0};
    }

    m_offset = 0;
    memset(m_buffer, 0, sizeof(float));
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
