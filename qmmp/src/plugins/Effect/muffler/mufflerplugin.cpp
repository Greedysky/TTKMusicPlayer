#include "mufflerplugin.h"

#include <QSettings>

MufflerPlugin *MufflerPlugin::m_instance = nullptr;

MufflerPlugin::MufflerPlugin()
    : Effect()
{
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_ratio = settings.value("Muffler/ratio", DEFAULT_RATIO).toInt();
}

void MufflerPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    m_mutex.lock();
    float *data = b->data;
    for(size_t i = 0; i < b->samples; i += 2)
    {
        data[i] -= data[i + 1] * m_ratio;
        data[i + 1] = data[i] * m_ratio;
    }
    m_mutex.unlock();
}

void MufflerPlugin::setRatio(int ratio)
{
    m_mutex.lock();
    m_ratio = ratio * 1.0 / DEFAULT_RATIO; // (0.0 - 1.0)
    m_mutex.unlock();
}

MufflerPlugin* MufflerPlugin::instance()
{
    return m_instance;
}
