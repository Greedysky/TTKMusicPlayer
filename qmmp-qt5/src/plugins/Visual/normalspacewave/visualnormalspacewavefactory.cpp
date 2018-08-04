#include <qmmp/qmmp.h>
#include "visualnormalspacewavefactory.h"
#include "normalspacewave.h"

const VisualProperties VisualNormalSpaceWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal SpaceWave Plugin");
    properties.shortName = "normalspacewave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualNormalSpaceWaveFactory::create(QWidget *parent)
{
    return new NormalSpaceWave(parent);
}
