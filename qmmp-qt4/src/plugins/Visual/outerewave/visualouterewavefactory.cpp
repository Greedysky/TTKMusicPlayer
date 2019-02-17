#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualouterewavefactory.h"
#include "outerewave.h"

VisualProperties VisualOuterEWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer EWave Plugin");
    properties.shortName = "outerewave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualOuterEWaveFactory::create(QWidget *parent)
{
    return new OuterEWave(parent);
}

Q_EXPORT_PLUGIN2(outerrayswave,VisualOuterEWaveFactory)
