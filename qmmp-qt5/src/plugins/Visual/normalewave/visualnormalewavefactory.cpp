#include <qmmp/qmmp.h>
#include "visualnormalewavefactory.h"
#include "normalewave.h"

const VisualProperties VisualNormalEWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal EWave Plugin");
    properties.shortName = "normalewave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualNormalEWaveFactory::create(QWidget *parent)
{
    return new NormalEWave(parent);
}
