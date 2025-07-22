#include "visualflowmadspinfactory.h"
#include "madspinplugin.h"

VisualProperties VisualFlowMadSpinFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow MadSpin Plugin");
    properties.shortName = "flowmadspin";
    return properties;
}

Visual *VisualFlowMadSpinFactory::create(QWidget *parent)
{
    return new MadSpinPlugin(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowmadspin, VisualFlowMadSpinFactory)
#endif
