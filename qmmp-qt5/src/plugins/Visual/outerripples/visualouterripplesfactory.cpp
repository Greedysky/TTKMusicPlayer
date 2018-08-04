#include <qmmp/qmmp.h>
#include "visualouterripplesfactory.h"
#include "outerripples.h"

const VisualProperties VisualOuterRipplesFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer Ripples Plugin");
    properties.shortName = "outerripples";
    properties.hasSettings = false;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualOuterRipplesFactory::create(QWidget *parent)
{
    return new OuterRipples(parent);
}
