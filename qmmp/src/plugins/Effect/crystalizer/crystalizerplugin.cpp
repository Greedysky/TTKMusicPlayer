#include "crystalizerplugin.h"

#include <QSettings>

CrystalizerPlugin *CrystalizerPlugin::m_instance = nullptr;

CrystalizerPlugin::CrystalizerPlugin()
    : Effect()
{
    m_instance = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_intensity = settings.value("Crystalizer/intensity", DEFAULT_INTENSITY).toUInt();
}

CrystalizerPlugin::~CrystalizerPlugin()
{
    m_instance = nullptr;
    delete[] m_buffer;
}

void CrystalizerPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    float *data = b->data;
    for(size_t i = 0; i < b->samples; )
    {
        for(int c = 0; c < channels(); ++c, ++i)
        {
            const float v = data[i];
            data[i] = v + (v - m_buffer[c]) * (m_intensity * 1.0f / DEFAULT_INTENSITY);
            m_buffer[c] = v;
        }
    }
    m_mutex.unlock();
}

void CrystalizerPlugin::configure(quint32 freq, ChannelMap map)
{
    if(channels() != map.count() || sampleRate() != freq)
    {
        Effect::configure(freq, map);

        delete[] m_buffer;
        m_buffer = new float[channels()]{0};
    }

    memset(m_buffer, 0, sizeof(float));
}

void CrystalizerPlugin::setIntensity(int intensity)
{
    m_mutex.lock();
    m_intensity = intensity;
    m_mutex.unlock();
}

CrystalizerPlugin* CrystalizerPlugin::instance()
{
    return m_instance;
}
