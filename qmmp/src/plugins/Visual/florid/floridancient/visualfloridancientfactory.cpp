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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridancient, VisualFloridAncientFactory)
#endif
