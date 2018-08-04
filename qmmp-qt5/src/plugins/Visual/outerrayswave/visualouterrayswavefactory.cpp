#include <qmmp/qmmp.h>
#include "visualouterrayswavefactory.h"
#include "outerrayswave.h"

const VisualProperties VisualOuterRaysWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer RaysWave Plugin");
    properties.shortName = "outerrayswave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualOuterRaysWaveFactory::create(QWidget *parent)
{
    return new OuterRaysWave(parent);
}
