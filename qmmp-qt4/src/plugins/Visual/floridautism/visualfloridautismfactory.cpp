#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfloridautismfactory.h"
#include "floridautism.h"

VisualProperties VisualFloridAutismFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Autism Plugin");
    properties.shortName = "floridautism";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridAutismFactory::create(QWidget *parent)
{
    return new FloridAutism(parent);
}

Q_EXPORT_PLUGIN2(floridautism,VisualFloridAutismFactory)
