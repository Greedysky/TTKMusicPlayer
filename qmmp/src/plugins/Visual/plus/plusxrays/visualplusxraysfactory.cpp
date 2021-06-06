#include "visualplusxraysfactory.h"
#include "plusxrays.h"

VisualProperties VisualPlusXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus XRays Plugin");
    properties.shortName = "plusxrays";
    return properties;
}

Visual *VisualPlusXRaysFactory::create(QWidget *parent)
{
    return new PlusXRays(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusxrays, VisualPlusXRaysFactory)
#endif
