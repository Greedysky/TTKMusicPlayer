#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualrayswavefactory.h"
#include "rayswave.h"

const VisualProperties VisualRaysWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("RaysWave Plugin");
    properties.shortName = "rayswave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualRaysWaveFactory::create(QWidget *parent)
{
    return new RaysWave(parent);
}

Q_EXPORT_PLUGIN2(ripples,VisualRaysWaveFactory)
