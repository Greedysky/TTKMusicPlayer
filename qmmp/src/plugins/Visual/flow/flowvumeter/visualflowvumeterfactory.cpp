#include "visualflowvumeterfactory.h"
#include "flowvumeter.h"

VisualProperties VisualFlowVUMeterFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow VU Meter Plugin");
    properties.shortName = "flowvumeter";
    return properties;
}

Visual *VisualFlowVUMeterFactory::create(QWidget *parent)
{
    return new FlowVUMeter(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowvumeter, VisualFlowVUMeterFactory)
#endif
