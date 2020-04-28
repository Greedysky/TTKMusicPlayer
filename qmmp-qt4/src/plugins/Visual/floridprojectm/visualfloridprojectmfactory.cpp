#include <QtPlugin>
#include "visualfloridprojectmfactory.h"
#include "projectmplugin.h"

VisualProperties VisualFloridProjectMFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid ProjectM Plugin");
    properties.shortName = "floridprojectm";
    properties.hasSettings = false;
    return properties;
}

Visual *VisualFloridProjectMFactory::create(QWidget *parent)
{
    return new ProjectMPlugin(parent);
}

Q_EXPORT_PLUGIN2(floridprojectm,VisualFloridProjectMFactory)
