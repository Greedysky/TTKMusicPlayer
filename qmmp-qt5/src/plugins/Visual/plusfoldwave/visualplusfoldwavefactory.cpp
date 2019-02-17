#include <qmmp/qmmp.h>
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
