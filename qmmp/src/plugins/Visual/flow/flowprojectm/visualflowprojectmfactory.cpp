#include "visualflowprojectmfactory.h"
#include "projectmplugin.h"

VisualProperties VisualFlowProjectMFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow ProjectM Plugin");
    properties.shortName = "flowprojectm";
    return properties;
}

Visual *VisualFlowProjectMFactory::create(QWidget *parent)
{
    return new ProjectMPlugin(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowprojectm, VisualFlowProjectMFactory)
#endif
