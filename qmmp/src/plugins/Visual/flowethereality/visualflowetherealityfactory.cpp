#include "visualflowetherealityfactory.h"
#include "flowethereality.h"

VisualProperties VisualFlowEtherealityFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Ethereality Plugin");
    properties.shortName = "flowethereality";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFlowEtherealityFactory::create(QWidget *parent)
{
    return new FlowEthereality(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowethereality, VisualFlowEtherealityFactory)
#endif
