#include "visualflowinfinityfactory.h"
#include "infinitywidget.h"

VisualProperties VisualFlowInfinityFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Infinity Plugin");
    properties.shortName = "flowinfinity";
    return properties;
}

Visual *VisualFlowInfinityFactory::create(QWidget *parent)
{
    return new InfinityWidget(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowinfinity, VisualFlowInfinityFactory)
#endif
