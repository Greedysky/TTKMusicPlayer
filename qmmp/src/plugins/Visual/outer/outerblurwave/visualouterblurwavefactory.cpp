#include "visualouterblurwavefactory.h"
#include "outerblurwave.h"

VisualProperties VisualOuterBlurWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Outer BlurWave Plugin");
    properties.shortName = "outerblurwave";
    return properties;
}

Visual *VisualOuterBlurWaveFactory::create(QWidget *parent)
{
    return new OuterBlurWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(outerblurwave, VisualOuterBlurWaveFactory)
#endif
