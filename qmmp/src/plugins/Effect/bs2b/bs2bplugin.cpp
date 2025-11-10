#include "bs2bplugin.h"

#include <QSettings>

Bs2bPlugin *Bs2bPlugin::m_instance = nullptr;

Bs2bPlugin::Bs2bPlugin()
    : Effect(),
      m_handler(bs2b_open())
{
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    bs2b_set_level(m_handler, settings.value("Bs2b/level", BS2B_DEFAULT_CLEVEL).toUInt());
}

Bs2bPlugin::~Bs2bPlugin()
{
    m_instance = nullptr;
    bs2b_close(m_handler);
}

void Bs2bPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    m_mutex.lock();
    bs2b_cross_feed_f(m_handler, b->data, b->samples / 2);
    m_mutex.unlock();
}

void Bs2bPlugin::configure(quint32 freq, ChannelMap map)
{
    Effect::configure(freq, map);
    bs2b_set_srate(m_handler, freq);
}

void Bs2bPlugin::setCrossfeedLevel(uint32_t level)
{
    m_mutex.lock();
    bs2b_set_level(m_handler, level);
    m_mutex.unlock();
}

Bs2bPlugin* Bs2bPlugin::instance()
{
    return m_instance;
}
