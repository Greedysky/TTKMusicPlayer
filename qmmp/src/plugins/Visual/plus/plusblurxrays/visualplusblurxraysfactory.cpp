#include "visualplusblurxraysfactory.h"
#include "plusblurxrays.h"

VisualProperties VisualPlusBlurXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus Blur XRays Plugin");
    properties.shortName = "plusblurxrays";
    return properties;
}

Visual *VisualPlusBlurXRaysFactory::create(QWidget *parent)
{
    return new PlusBlurXRays(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusblurxrays, VisualPlusBlurXRaysFactory)
#endif
