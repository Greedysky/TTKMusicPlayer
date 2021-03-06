#include "visualnormalewavefactory.h"
#include "normalewave.h"

VisualProperties VisualNormalEWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal EWave Plugin");
    properties.shortName = "normalewave";
    return properties;
}

Visual *VisualNormalEWaveFactory::create(QWidget *parent)
{
    return new NormalEWave(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalewave, VisualNormalEWaveFactory)
#endif
