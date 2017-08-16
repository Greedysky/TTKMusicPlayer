#include <qmmp/qmmp.h>
#include "visualripplesfactory.h"
#include "ripples.h"

const VisualProperties VisualRipplesFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Ripples Plugin");
    properties.shortName = "ripples";
    properties.hasSettings = false;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualRipplesFactory::create(QWidget *parent)
{
    return new Ripples(parent);
}
