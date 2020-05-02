#include "visualwavemultifactory.h"
#include "wavemulti.h"

VisualProperties VisualWaveMultiFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Multi Plugin");
    properties.shortName = "wavemulti";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualWaveMultiFactory::create(QWidget *parent)
{
    return new WaveMulti(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavemulti, VisualWaveMultiFactory)
#endif
