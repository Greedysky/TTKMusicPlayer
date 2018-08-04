#include <qmmp/qmmp.h>
#include "visualplusvolumewavefactory.h"
#include "plusvolumewave.h"

const VisualProperties VisualPlusVolumeWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus VolumeWave Plugin");
    properties.shortName = "plusvolumewave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPlusVolumeWaveFactory::create(QWidget *parent)
{
    return new PlusVolumeWave(parent);
}
