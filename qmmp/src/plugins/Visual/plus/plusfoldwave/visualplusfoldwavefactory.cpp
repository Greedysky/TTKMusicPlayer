#include "visualplusfoldwavefactory.h"
#include "plusfoldwave.h"

VisualProperties VisualPlusFoldWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus FoldWave Plugin");
    properties.shortName = "plusfoldwave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusFoldWaveFactory::create(QWidget *parent)
{
    return new PlusFoldWave(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusfoldwave, VisualPlusFoldWaveFactory)
#endif
