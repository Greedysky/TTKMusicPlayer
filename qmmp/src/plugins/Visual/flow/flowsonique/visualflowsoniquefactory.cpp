#include "visualflowsoniquefactory.h"
#include "soniqueplugin.h"

VisualProperties VisualFlowSoniqueFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Sonique Plugin");
    properties.shortName = "flowsonique";
    return properties;
}

Visual *VisualFlowSoniqueFactory::create(QWidget *parent)
{
    return new SoniquePlugin(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowsonique, VisualFlowSoniqueFactory)
#endif
