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
