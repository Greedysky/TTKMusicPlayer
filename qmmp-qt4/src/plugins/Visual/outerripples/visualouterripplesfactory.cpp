#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualouterripplesfactory.h"
#include "outerripples.h"

VisualProperties VisualOuterRipplesFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer Ripples Plugin");
    properties.shortName = "outerripples";
    properties.hasSettings = false;
    return properties;
}

Visual *VisualOuterRipplesFactory::create(QWidget *parent)
{
    return new OuterRipples(parent);
}

Q_EXPORT_PLUGIN2(outerripples,VisualOuterRipplesFactory)
