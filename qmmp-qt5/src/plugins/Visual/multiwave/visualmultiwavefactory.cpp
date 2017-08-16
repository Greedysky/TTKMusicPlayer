#include <qmmp/qmmp.h>
#include "visualmultiwavefactory.h"
#include "multiwave.h"

const VisualProperties VisualMultiWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("MultiWave Plugin");
    properties.shortName = "multiwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualMultiWaveFactory::create(QWidget *parent)
{
    return new MultiWave(parent);
}
