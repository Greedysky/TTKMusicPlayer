#include "visualnormalflowwavefactory.h"
#include "normalflowwave.h"

VisualProperties VisualNormalFlowWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal FlowWave Plugin");
    properties.shortName = "normalflowwave";
    return properties;
}

Visual *VisualNormalFlowWaveFactory::create(QWidget *parent)
{
    return new NormalFlowWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalflowwave, VisualNormalFlowWaveFactory)
#endif
