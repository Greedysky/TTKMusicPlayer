#include "outputwaveout.h"
#include "outputwaveoutfactory.h"

OutputProperties OutputWaveOutFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("WaveOut Plugin");
    properties.hasSettings = false;
    properties.shortName = "waveout";
    return properties;
}

Output* OutputWaveOutFactory::create()
{
    return new OutputWaveOut();
}

Volume *OutputWaveOutFactory::createVolume()
{
    VolumeWaveOut *vol = new VolumeWaveOut();
    if(!vol->isSupported())
    {
        qDebug("OutputWaveOutFactory: device doesn't support volume control");
        delete vol;
        return nullptr;
    }
    return vol;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(waveout, OutputWaveOutFactory)
#endif
