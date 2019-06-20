#include <qmmp/qmmp.h>
#include "visualfloridelectricfactory.h"
#include "floridelectric.h"

VisualProperties VisualFloridElectricFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Electric Plugin");
    properties.shortName = "floridelectric";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridElectricFactory::create(QWidget *parent)
{
    return new FloridElectric(parent);
}
