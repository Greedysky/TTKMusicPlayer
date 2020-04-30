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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridprojectm, VisualFloridProjectMFactory)
#endif
