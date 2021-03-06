#include "visualouterblurwavefactory.h"
#include "outerblurwave.h"

VisualProperties VisualOuterBlurWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer BlurWave Plugin");
    properties.shortName = "outerblurwave";
    return properties;
}

Visual *VisualOuterBlurWaveFactory::create(QWidget *parent)
{
    return new OuterBlurWave(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerblurwave, VisualOuterBlurWaveFactory)
#endif
