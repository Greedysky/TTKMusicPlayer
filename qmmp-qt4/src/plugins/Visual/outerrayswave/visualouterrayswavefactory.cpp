#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualouterrayswavefactory.h"
#include "outerrayswave.h"

VisualProperties VisualOuterRaysWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer RaysWave Plugin");
    properties.shortName = "outerrayswave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualOuterRaysWaveFactory::create(QWidget *parent)
{
    return new OuterRaysWave(parent);
}

Q_EXPORT_PLUGIN2(outerrayswave,VisualOuterRaysWaveFactory)
