#include <qmmp/qmmp.h>
#include "visualplusmonowavefactory.h"
#include "plusmonowave.h"

const VisualProperties VisualPlusMonoWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus MonoWave Plugin");
    properties.shortName = "plusmonowave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPlusMonoWaveFactory::create(QWidget *parent)
{
    return new PlusMonoWave(parent);
}
