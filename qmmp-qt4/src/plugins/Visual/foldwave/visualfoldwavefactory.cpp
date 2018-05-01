#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfoldwavefactory.h"
#include "foldwave.h"

const VisualProperties VisualFoldWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("FoldWave Plugin");
    properties.shortName = "foldwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualFoldWaveFactory::create(QWidget *parent)
{
    return new FoldWave(parent);
}

Q_EXPORT_PLUGIN2(ewave,VisualFoldWaveFactory)
