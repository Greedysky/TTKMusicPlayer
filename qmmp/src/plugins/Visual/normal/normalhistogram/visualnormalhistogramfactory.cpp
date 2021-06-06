#include "visualnormalhistogramfactory.h"
#include "normalhistogram.h"

VisualProperties VisualNormalHistogramFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Histogram Plugin");
    properties.shortName = "normalhistogram";
    return properties;
}

Visual *VisualNormalHistogramFactory::create(QWidget *parent)
{
    return new NormalHistogram(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalhistogram, VisualNormalHistogramFactory)
#endif
