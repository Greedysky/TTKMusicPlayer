#include "visualwavevoicefactory.h"
#include "wavevoice.h"

VisualProperties VisualWaveVoiceFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Voice Plugin");
    properties.shortName = "wavevoice";
    return properties;
}

Visual *VisualWaveVoiceFactory::create(QWidget *parent)
{
    return new WaveVoice(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavevoice, VisualWaveVoiceFactory)
#endif
