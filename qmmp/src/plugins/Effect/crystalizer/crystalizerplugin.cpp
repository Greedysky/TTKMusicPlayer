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
    if(m_buffer)
    {
        delete[] m_buffer;
    }
}

void CrystalizerPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    float *data = b->data;
    for(size_t i = 0; i < b->samples; )
    {
        for(int c = 0; c < m_chan; c++, i++)
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
    m_chan = map.count();
    if(m_buffer)
    {
        delete[] m_buffer;
    }
    m_buffer = new float[m_chan]{0};
    Effect::configure(freq, map);
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
