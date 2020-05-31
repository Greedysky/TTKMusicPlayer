#include "visualplusxraysfactory.h"
#include "plusxrays.h"

VisualProperties VisualPlusXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus XRays Plugin");
    properties.shortName = "plusxrays";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusXRaysFactory::create(QWidget *parent)
{
    return new PlusXRays(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusxrays, VisualPlusXRaysFactory)
#endif
