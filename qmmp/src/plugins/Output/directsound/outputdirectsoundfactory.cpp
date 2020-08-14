#include "outputdirectsound.h"
#include "outputdirectsoundfactory.h"

OutputProperties OutputDirectSoundFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("DirectSound Plugin");
    properties.hasSettings = false;
    properties.shortName = "directsound";
    return properties;
}

Output* OutputDirectSoundFactory::create()
{
    return new OutputDirectSound();
}

Volume *OutputDirectSoundFactory::createVolume()
{
    return new VolumeDirectSound();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(directsound, OutputDirectSoundFactory)
#endif
