#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfloridsurroundfactory.h"
#include "floridsurround.h"

VisualProperties VisualFloridSurroundFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Surround Plugin");
    properties.shortName = "floridsurround";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridSurroundFactory::create(QWidget *parent)
{
    return new FloridSurround(parent);
}

Q_EXPORT_PLUGIN2(floridsurround,VisualFloridSurroundFactory)
