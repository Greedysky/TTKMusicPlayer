#include "visualplusspacewavefactory.h"
#include "plusspacewave.h"

VisualProperties VisualPLusSpaceWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus SpaceWave Plugin");
    properties.shortName = "plusspacewave";
    return properties;
}

Visual *VisualPLusSpaceWaveFactory::create(QWidget *parent)
{
    return new PlusSpaceWave(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(plusspacewave, VisualPLusSpaceWaveFactory)
#endif
