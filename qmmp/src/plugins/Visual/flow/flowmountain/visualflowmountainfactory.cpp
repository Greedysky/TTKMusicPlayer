#include "visualflowmountainfactory.h"
#include "mountainplugin.h"

VisualProperties VisualFlowMountainFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Mountain Plugin");
    properties.shortName = "flowmountain";
    return properties;
}

Visual *VisualFlowMountainFactory::create(QWidget *parent)
{
    return new MountainPlugin(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowmountain, VisualFlowMountainFactory)
#endif
