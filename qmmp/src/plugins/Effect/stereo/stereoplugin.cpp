#include "stereoplugin.h"

#include <QSettings>
#include <math.h>

StereoPlugin *StereoPlugin::m_instance = nullptr;

StereoPlugin::StereoPlugin()
    : Effect()
{
    m_instance = this;
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_intensity = settings.value("Stereo/intensity", 4.0).toDouble();
}

StereoPlugin::~StereoPlugin()
{
    m_instance = nullptr;
}

void StereoPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    m_mutex.lock();
    double average = 0, ldiff = 0, rdiff = 0, offset = 0;

    float *data = b->data;
    for(size_t i = 0; i < b->samples; i += 2)
    {
        average = (data[i] + data[i + 1]) / 2;
        ldiff = data[i] - average;
        rdiff = data[i + 1] - average;

        offset = average + ldiff * m_intensity;
        data[i] = qBound(-1.0, offset, 1.0);
        offset = average + rdiff * m_intensity;
        data[i + 1] = qBound(-1.0, offset, 1.0);
    }
    m_mutex.unlock();
}

void StereoPlugin::setIntensity(double level)
{
    m_mutex.lock();
    m_intensity = level;
    m_mutex.unlock();
}

StereoPlugin* StereoPlugin::instance()
{
    return m_instance;
}
