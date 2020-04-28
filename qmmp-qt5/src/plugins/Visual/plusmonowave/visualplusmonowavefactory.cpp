#include "visualplusmonowavefactory.h"
#include "plusmonowave.h"

VisualProperties VisualPlusMonoWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus MonoWave Plugin");
    properties.shortName = "plusmonowave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusMonoWaveFactory::create(QWidget *parent)
{
    return new PlusMonoWave(parent);
}
