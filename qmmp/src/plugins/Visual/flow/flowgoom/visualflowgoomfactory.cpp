#include "visualflowgoomfactory.h"
#include "goomwidget.h"

VisualProperties VisualFlowGoomFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Goom Plugin");
    properties.shortName = "flowgoom";
    return properties;
}

Visual *VisualFlowGoomFactory::create(QWidget *parent)
{
    return new GoomWidget(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowgoom, VisualFlowGoomFactory)
#endif
