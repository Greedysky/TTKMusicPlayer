#include "visualwavevoicefactory.h"
#include "wavevoice.h"

VisualProperties VisualWaveVoiceFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Voice Plugin");
    properties.shortName = "wavevoice";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualWaveVoiceFactory::create(QWidget *parent)
{
    return new WaveVoice(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavevoice, VisualWaveVoiceFactory)
#endif
