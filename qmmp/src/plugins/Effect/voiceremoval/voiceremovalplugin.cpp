#include "voiceremovalplugin.h"

VoiceRemovalPlugin::VoiceRemovalPlugin()
    : Effect()
{

}

VoiceRemovalPlugin::~VoiceRemovalPlugin()
{

}

void VoiceRemovalPlugin::applyEffect(Buffer *b)
{
    if(m_chan != 2)
        return;

    float *data = b->data;
    for(uint i = 0; i < b->samples; i += 2)
    {
        data[i] -= data[i + 1];
        data[i + 1] = data[i];
    }
}

void VoiceRemovalPlugin::configure(quint32 freq, ChannelMap map)
{
    m_chan = map.count();
    Effect::configure(freq, map);
}
