#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfloridmeterfactory.h"
#include "floridmeter.h"

VisualProperties VisualFloridMeterFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Meter Plugin");
    properties.shortName = "floridmeter";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridMeterFactory::create(QWidget *parent)
{
    return new FloridMeter(parent);
}

Q_EXPORT_PLUGIN2(floridmeter,VisualFloridMeterFactory)
