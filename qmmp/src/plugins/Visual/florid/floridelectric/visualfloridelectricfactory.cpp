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

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridelectric, VisualFloridElectricFactory)
#endif
