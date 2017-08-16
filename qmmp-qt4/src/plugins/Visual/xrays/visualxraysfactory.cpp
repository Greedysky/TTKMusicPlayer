#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualxraysfactory.h"
#include "xrays.h"

const VisualProperties VisualXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("XRays Plugin");
    properties.shortName = "xrays";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualXRaysFactory::create(QWidget *parent)
{
    return new XRays(parent);
}

Q_EXPORT_PLUGIN2(ripples,VisualXRaysFactory)
