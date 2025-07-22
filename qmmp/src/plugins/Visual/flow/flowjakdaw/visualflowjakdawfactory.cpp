#include "visualflowjakdawfactory.h"
#include "jakdawwidget.h"

VisualProperties VisualFlowJakdawFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Jakdaw Plugin");
    properties.shortName = "flowjakdaw";
    return properties;
}

Visual *VisualFlowJakdawFactory::create(QWidget *parent)
{
    return new JakdawWidget(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowjakdaw, VisualFlowJakdawFactory)
#endif
