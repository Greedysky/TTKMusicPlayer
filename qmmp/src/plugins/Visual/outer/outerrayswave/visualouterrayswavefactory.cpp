#include "visualouterrayswavefactory.h"
#include "outerrayswave.h"

VisualProperties VisualOuterRaysWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer RaysWave Plugin");
    properties.shortName = "outerrayswave";
    return properties;
}

Visual *VisualOuterRaysWaveFactory::create(QWidget *parent)
{
    return new OuterRaysWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerrayswave, VisualOuterRaysWaveFactory)
#endif
