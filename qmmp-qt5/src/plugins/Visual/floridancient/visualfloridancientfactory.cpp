#include <qmmp/qmmp.h>
#include "visualfloridancientfactory.h"
#include "floridancient.h"

VisualProperties VisualFloridAncientFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Ancient Plugin");
    properties.shortName = "floridancient";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridAncientFactory::create(QWidget *parent)
{
    return new FloridAncient(parent);
}
