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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridbass, VisualFloridBassFactory)
#endif
