#include "visualouterrayswavefactory.h"
#include "outerrayswave.h"

VisualProperties VisualOuterRaysWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer RaysWave Plugin");
    properties.shortName = "outerrayswave";
    return properties;
}

Visual *VisualOuterRaysWaveFactory::create(QWidget *parent)
{
    return new OuterRaysWave(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerrayswave, VisualOuterRaysWaveFactory)
#endif
