#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfloridbassfactory.h"
#include "floridbass.h"

VisualProperties VisualFloridBassFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Bass Plugin");
    properties.shortName = "floridbass";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridBassFactory::create(QWidget *parent)
{
    return new FloridBass(parent);
}

Q_EXPORT_PLUGIN2(floridbass,VisualFloridBassFactory)
