#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualnormalhistogramfactory.h"
#include "normalhistogram.h"

VisualProperties VisualNormalHistogramFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Histogram Plugin");
    properties.shortName = "normalhistogram";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualNormalHistogramFactory::create(QWidget *parent)
{
    return new NormalHistogram(parent);
}

Q_EXPORT_PLUGIN2(normalhistogram,VisualNormalHistogramFactory)
