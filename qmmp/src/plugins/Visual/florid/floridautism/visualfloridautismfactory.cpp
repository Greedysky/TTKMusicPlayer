#include "visualfloridautismfactory.h"
#include "floridautism.h"

VisualProperties VisualFloridAutismFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Autism Plugin");
    properties.shortName = "floridautism";
    return properties;
}

Visual *VisualFloridAutismFactory::create(QWidget *parent)
{
    return new FloridAutism(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridautism, VisualFloridAutismFactory)
#endif
