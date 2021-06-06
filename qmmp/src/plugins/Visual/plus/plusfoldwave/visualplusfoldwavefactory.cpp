#include "visualplusfoldwavefactory.h"
#include "plusfoldwave.h"

VisualProperties VisualPlusFoldWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus FoldWave Plugin");
    properties.shortName = "plusfoldwave";
    return properties;
}

Visual *VisualPlusFoldWaveFactory::create(QWidget *parent)
{
    return new PlusFoldWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusfoldwave, VisualPlusFoldWaveFactory)
#endif
