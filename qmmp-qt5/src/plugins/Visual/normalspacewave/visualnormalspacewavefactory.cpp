#include "visualnormalspacewavefactory.h"
#include "normalspacewave.h"

VisualProperties VisualNormalSpaceWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal SpaceWave Plugin");
    properties.shortName = "normalspacewave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualNormalSpaceWaveFactory::create(QWidget *parent)
{
    return new NormalSpaceWave(parent);
}
