#include "visualfloridetherealityfactory.h"
#include "floridethereality.h"

VisualProperties VisualFloridEtherealityFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Ethereality Plugin");
    properties.shortName = "floridethereality";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridEtherealityFactory::create(QWidget *parent)
{
    return new FloridEthereality(parent);
}
