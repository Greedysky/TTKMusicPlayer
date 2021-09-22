#include "monoplugin.h"

MonoPlugin::MonoPlugin()
    : Effect()
{

}

MonoPlugin::~MonoPlugin()
{

}

void MonoPlugin::applyEffect(Buffer *b)
{
    if(m_chan != 2)
        return;

    m_mutex.lock();
    float *data = b->data;

    for(uint i = 0; i < b->samples; i += 2)
    {
        m_avg = (data[i] + data[i + 1]) / 2;
        m_avg = qBound(-1.0, m_avg, 1.0);
        data[i] = data[i + 1] = m_avg;
    }

    m_mutex.unlock();
}

void MonoPlugin::configure(quint32 freq, ChannelMap map)
{
    m_chan = map.count();
    Effect::configure(freq, map);
}
