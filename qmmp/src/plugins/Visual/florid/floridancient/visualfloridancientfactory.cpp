#include "visualfloridancientfactory.h"
#include "floridancient.h"

VisualProperties VisualFloridAncientFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Ancient Plugin");
    properties.shortName = "floridancient";
    return properties;
}

Visual *VisualFloridAncientFactory::create(QWidget *parent)
{
    return new FloridAncient(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridancient, VisualFloridAncientFactory)
#endif
