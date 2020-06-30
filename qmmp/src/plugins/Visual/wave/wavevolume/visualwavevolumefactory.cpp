#include "visualwavevolumefactory.h"
#include "wavevolume.h"

VisualProperties VisualWaveVolumeFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Volume Plugin");
    properties.shortName = "wavevolume";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualWaveVolumeFactory::create(QWidget *parent)
{
    return new WaveVolume(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavevolume, VisualWaveVolumeFactory)
#endif
