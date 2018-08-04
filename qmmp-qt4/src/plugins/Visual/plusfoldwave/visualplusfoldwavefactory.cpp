#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualplusfoldwavefactory.h"
#include "plusfoldwave.h"

const VisualProperties VisualPlusFoldWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus FoldWave Plugin");
    properties.shortName = "plusfoldwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPlusFoldWaveFactory::create(QWidget *parent)
{
    return new PlusFoldWave(parent);
}

Q_EXPORT_PLUGIN2(plusfoldwave,VisualPlusFoldWaveFactory)
