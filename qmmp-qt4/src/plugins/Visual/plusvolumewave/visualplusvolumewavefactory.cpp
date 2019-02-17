#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualplusvolumewavefactory.h"
#include "plusvolumewave.h"

VisualProperties VisualPlusVolumeWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus VolumeWave Plugin");
    properties.shortName = "plusvolumewave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusVolumeWaveFactory::create(QWidget *parent)
{
    return new PlusVolumeWave(parent);
}

Q_EXPORT_PLUGIN2(plusvolumewave,VisualPlusVolumeWaveFactory)
