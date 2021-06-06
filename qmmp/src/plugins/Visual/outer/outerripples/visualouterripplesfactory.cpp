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

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerripples, VisualOuterRipplesFactory)
#endif
