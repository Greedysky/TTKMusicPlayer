#include "visualfloridelectricfactory.h"
#include "floridelectric.h"

VisualProperties VisualFloridElectricFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Electric Plugin");
    properties.shortName = "floridelectric";
    return properties;
}

Visual *VisualFloridElectricFactory::create(QWidget *parent)
{
    return new FloridElectric(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridelectric, VisualFloridElectricFactory)
#endif
