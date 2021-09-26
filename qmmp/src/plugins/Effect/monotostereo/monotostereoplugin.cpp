#include "monotostereoplugin.h"

MonoToStereoPlugin::MonoToStereoPlugin()
    : Effect()
{

}

MonoToStereoPlugin::~MonoToStereoPlugin()
{
    if(m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

void MonoToStereoPlugin::applyEffect(Buffer *b)
{
    if(m_enabled)
    {
        if(m_size < b->samples)
        {
            if(m_buffer)
            {
                delete[] m_buffer;
            }
            m_buffer = new float[b->samples];
        }

        memcpy(m_buffer, b->data, b->samples * sizeof(float));
        b->samples *= 2;

        if(b->samples > b->size)
        {
            delete[] b->data;
            b->size = b->samples;
            b->data = new float[b->size];
        }

        for(size_t i = 0; i < b->samples; ++i)
        {
            b->data[i] = m_buffer[i >> 1];
        }
    }
}

void MonoToStereoPlugin::configure(quint32 freq, ChannelMap map)
{
    if(map.count() == 1)
    {
        m_enabled = true;
        Effect::configure(freq, ChannelMap(2));
    }
    else
    {
        m_enabled = false;
        Effect::configure(freq, map);
    }
}
