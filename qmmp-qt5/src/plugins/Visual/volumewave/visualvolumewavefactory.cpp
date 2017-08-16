#include <qmmp/qmmp.h>
#include "visualvolumewavefactory.h"
#include "volumewave.h"

const VisualProperties VisualVolumeWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("VolumeWave Plugin");
    properties.shortName = "volumewave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualVolumeWaveFactory::create(QWidget *parent)
{
    return new VolumeWave(parent);
}
