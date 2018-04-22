#include <qmmp/qmmp.h>
#include "visualpointxraysfactory.h"
#include "pointxrays.h"

const VisualProperties VisualPointXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("PointXRays Plugin");
    properties.shortName = "pointxrays";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPointXRaysFactory::create(QWidget *parent)
{
    return new PointXRays(parent);
}
