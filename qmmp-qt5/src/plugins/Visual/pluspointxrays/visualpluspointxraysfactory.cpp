#include "visualpluspointxraysfactory.h"
#include "pluspointxrays.h"

VisualProperties VisualPlusPointXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus PointXRays Plugin");
    properties.shortName = "pluspointxrays";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusPointXRaysFactory::create(QWidget *parent)
{
    return new PlusPointXRays(parent);
}
