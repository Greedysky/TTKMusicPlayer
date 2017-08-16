#include <qmmp/qmmp.h>
#include "visualgwavefactory.h"
#include "gwave.h"

const VisualProperties VisualGWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("GWave Plugin");
    properties.shortName = "gwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualGWaveFactory::create(QWidget *parent)
{
    return new GWave(parent);
}
