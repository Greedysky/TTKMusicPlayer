#include "visualwavecrestfactory.h"
#include "wavecrest.h"

VisualProperties VisualWaveCrestFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Crest Plugin");
    properties.shortName = "wavecrest";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualWaveCrestFactory::create(QWidget *parent)
{
    return new WaveCrest(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavecrest, VisualWaveCrestFactory)
#endif
