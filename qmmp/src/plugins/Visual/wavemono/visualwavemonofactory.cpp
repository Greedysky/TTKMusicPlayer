#include "visualwavemonofactory.h"
#include "wavemono.h"

VisualProperties VisualWaveMonoFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Mono Plugin");
    properties.shortName = "wavemono";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualWaveMonoFactory::create(QWidget *parent)
{
    return new WaveMono(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavemono, VisualWaveMonoFactory)
#endif
