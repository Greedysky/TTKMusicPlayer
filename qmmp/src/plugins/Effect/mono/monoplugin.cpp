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
    if(channels() != 2)
    {
        return;
    }

    float *data = b->data;
    for(size_t i = 0; i < b->samples; i += 2)
    {
        m_average = (data[i] + data[i + 1]) / 2;
        m_average = qBound(-1.0, m_average, 1.0);
        data[i] = data[i + 1] = m_average;
    }
}
