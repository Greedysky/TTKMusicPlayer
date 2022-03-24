#include "visualflowsoniquefactory.h"
#include "soniquewidget.h"

VisualProperties VisualFlowSoniqueFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Sonique Plugin");
    properties.shortName = "flowsonique";
    return properties;
}

Visual *VisualFlowSoniqueFactory::create(QWidget *parent)
{
    return new SoniqueWidget(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowsonique, VisualFlowSoniqueFactory)
#endif
