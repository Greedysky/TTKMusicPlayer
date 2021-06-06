#include "visualwavevolumefactory.h"
#include "wavevolume.h"

VisualProperties VisualWaveVolumeFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Volume Plugin");
    properties.shortName = "wavevolume";
    return properties;
}

Visual *VisualWaveVolumeFactory::create(QWidget *parent)
{
    return new WaveVolume(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavevolume, VisualWaveVolumeFactory)
#endif
