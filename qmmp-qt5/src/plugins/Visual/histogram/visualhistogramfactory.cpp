#include <qmmp/qmmp.h>
#include "visualhistogramfactory.h"
#include "histogram.h"

const VisualProperties VisualHistogramFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Histogram Plugin");
    properties.shortName = "histogram";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualHistogramFactory::create(QWidget *parent)
{
    return new Histogram(parent);
}
