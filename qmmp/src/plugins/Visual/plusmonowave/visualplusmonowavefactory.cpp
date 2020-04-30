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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusmonowave, VisualPlusMonoWaveFactory)
#endif
