#include "visualflowmountainfactory.h"
#include "mountainplugin.h"

VisualProperties VisualFlowMountainFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Mountain Plugin");
    properties.shortName = "flowmountain";
    properties.hasSettings = false;
    return properties;
}

Visual *VisualFlowMountainFactory::create(QWidget *parent)
{
    return new MountainPlugin(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowmountain, VisualFlowMountainFactory)
#endif
