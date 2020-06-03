#include "visualplusblurxraysfactory.h"
#include "plusblurxrays.h"

VisualProperties VisualPlusBlurXRaysFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus Blur XRays Plugin");
    properties.shortName = "plusblurxrays";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusBlurXRaysFactory::create(QWidget *parent)
{
    return new PlusBlurXRays(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusblurxrays, VisualPlusBlurXRaysFactory)
#endif
