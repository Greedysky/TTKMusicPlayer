#include "visualflowprojectmfactory.h"
#include "projectmplugin.h"

VisualProperties VisualFlowProjectMFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow ProjectM Plugin");
    properties.shortName = "flowprojectm";
    properties.hasSettings = false;
    return properties;
}

Visual *VisualFlowProjectMFactory::create(QWidget *parent)
{
    return new ProjectMPlugin(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowprojectm, VisualFlowProjectMFactory)
#endif
