#include "mufflerplugin.h"

MufflerPlugin::MufflerPlugin()
    : Effect()
{

}

MufflerPlugin::~MufflerPlugin()
{

}

void MufflerPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    float *data = b->data;
    for(uint i = 0; i < b->samples; i += 2)
    {
        data[i] -= data[i + 1];
        data[i + 1] = data[i];
    }
}
