#include <qmmp/qmmp.h>
#include "visualfwavefactory.h"
#include "fwave.h"

const VisualProperties VisualFWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("FWave Plugin");
    properties.shortName = "fwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualFWaveFactory::create(QWidget *parent)
{
    return new FWave(parent);
}
