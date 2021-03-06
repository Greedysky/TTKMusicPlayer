#include "visualouterripplesfactory.h"
#include "outerripples.h"

VisualProperties VisualOuterRipplesFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer Ripples Plugin");
    properties.shortName = "outerripples";
    return properties;
}

Visual *VisualOuterRipplesFactory::create(QWidget *parent)
{
    return new OuterRipples(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerripples, VisualOuterRipplesFactory)
#endif
