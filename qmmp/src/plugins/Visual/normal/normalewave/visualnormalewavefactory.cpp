#include "visualnormalewavefactory.h"
#include "normalewave.h"

VisualProperties VisualNormalEWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal EWave Plugin");
    properties.shortName = "normalewave";
    return properties;
}

Visual *VisualNormalEWaveFactory::create(QWidget *parent)
{
    return new NormalEWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalewave, VisualNormalEWaveFactory)
#endif
