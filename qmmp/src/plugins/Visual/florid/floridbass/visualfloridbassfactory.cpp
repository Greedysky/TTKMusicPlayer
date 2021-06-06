#include "visualfloridbassfactory.h"
#include "floridbass.h"

VisualProperties VisualFloridBassFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Bass Plugin");
    properties.shortName = "floridbass";
    return properties;
}

Visual *VisualFloridBassFactory::create(QWidget *parent)
{
    return new FloridBass(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridbass, VisualFloridBassFactory)
#endif
