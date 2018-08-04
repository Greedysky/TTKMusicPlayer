#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualnormalhistogramfactory.h"
#include "normalhistogram.h"

const VisualProperties VisualNormalHistogramFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Histogram Plugin");
    properties.shortName = "normalhistogram";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualNormalHistogramFactory::create(QWidget *parent)
{
    return new NormalHistogram(parent);
}

Q_EXPORT_PLUGIN2(normalhistogram,VisualNormalHistogramFactory)
