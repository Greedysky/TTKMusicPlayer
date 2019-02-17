#include <qmmp/qmmp.h>
#include "visualnormalflowwavefactory.h"
#include "normalflowwave.h"

VisualProperties VisualNormalFlowWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal FlowWave Plugin");
    properties.shortName = "normalflowwave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualNormalFlowWaveFactory::create(QWidget *parent)
{
    return new NormalFlowWave(parent);
}
