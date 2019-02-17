#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualflashmeterfactory.h"
#include "flashmeter.h"

VisualProperties VisualFlashMeterFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flash Meter Plugin");
    properties.shortName = "flashmeter";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFlashMeterFactory::create(QWidget *parent)
{
    return new FlashMeter(parent);
}

Q_EXPORT_PLUGIN2(flashmeter,VisualFlashMeterFactory)
