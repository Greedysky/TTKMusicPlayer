#include "visualflowetherealityfactory.h"
#include "flowethereality.h"

VisualProperties VisualFlowEtherealityFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Ethereality Plugin");
    properties.shortName = "flowethereality";
    return properties;
}

Visual *VisualFlowEtherealityFactory::create(QWidget *parent)
{
    return new FlowEthereality(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowethereality, VisualFlowEtherealityFactory)
#endif
