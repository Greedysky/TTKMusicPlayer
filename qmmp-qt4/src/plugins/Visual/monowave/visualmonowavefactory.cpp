#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualmonowavefactory.h"
#include "monowave.h"

const VisualProperties VisualMonoWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("MonoWave Plugin");
    properties.shortName = "monowave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualMonoWaveFactory::create(QWidget *parent)
{
    return new MonoWave(parent);
}

Q_EXPORT_PLUGIN2(monowave,VisualMonoWaveFactory)
