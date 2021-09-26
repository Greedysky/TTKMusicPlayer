#include <QSettings>
#include <math.h>
#include "stereoplugin.h"

StereoPlugin *StereoPlugin::m_instance = nullptr;

StereoPlugin::StereoPlugin()
    : Effect()
{
    m_instance = this;
}

StereoPlugin::~StereoPlugin()
{
    m_instance = nullptr;
}

void StereoPlugin::applyEffect(Buffer *b)
{
    if(m_chan != 2)
        return;

    m_mutex.lock();
    float *data = b->data;
    for(uint i = 0; i < b->samples; i += 2)
    {
        m_avg = (data[i] + data[i + 1]) / 2;
        m_ldiff = data[i] - m_avg;
        m_rdiff = data[i + 1] - m_avg;

        m_offset = m_avg + m_ldiff * m_mul;
        data[i] = qBound(-1.0, m_offset, 1.0);
        m_offset = m_avg + m_rdiff * m_mul;
        data[i + 1] = qBound(-1.0, m_offset, 1.0);
    }
    m_mutex.unlock();
}

void StereoPlugin::configure(quint32 freq, ChannelMap map)
{
    m_chan = map.count();
    Effect::configure(freq, map);
}

void StereoPlugin::setIntensity(double level)
{
    m_mutex.lock();
    m_mul = level;
    m_mutex.unlock();
}

StereoPlugin* StereoPlugin::instance()
{
    return m_instance;
}
