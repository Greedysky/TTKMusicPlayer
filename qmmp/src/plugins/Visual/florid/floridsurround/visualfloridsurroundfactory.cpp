#include "visualfloridsurroundfactory.h"
#include "floridsurround.h"

VisualProperties VisualFloridSurroundFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Surround Plugin");
    properties.shortName = "floridsurround";
    return properties;
}

Visual *VisualFloridSurroundFactory::create(QWidget *parent)
{
    return new FloridSurround(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridsurround, VisualFloridSurroundFactory)
#endif
