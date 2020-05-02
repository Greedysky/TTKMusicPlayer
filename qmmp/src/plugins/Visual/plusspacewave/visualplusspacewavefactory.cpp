#include "visualplusspacewavefactory.h"
#include "plusspacewave.h"

VisualProperties VisualPLusSpaceWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus SpaceWave Plugin");
    properties.shortName = "plusspacewave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPLusSpaceWaveFactory::create(QWidget *parent)
{
    return new PlusSpaceWave(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusspacewave, VisualPLusSpaceWaveFactory)
#endif
