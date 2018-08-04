#include <qmmp/qmmp.h>
#include "visualpluspointxraysfactory.h"
#include "pluspointxrays.h"

const VisualProperties VisualPlusPointXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus PointXRays Plugin");
    properties.shortName = "pluspointxrays";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPlusPointXRaysFactory::create(QWidget *parent)
{
    return new PlusPointXRays(parent);
}
