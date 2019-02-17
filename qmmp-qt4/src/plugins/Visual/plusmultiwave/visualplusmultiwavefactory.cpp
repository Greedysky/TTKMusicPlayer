#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualplusmultiwavefactory.h"
#include "plusmultiwave.h"

VisualProperties VisualPlusMultiWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus MultiWave Plugin");
    properties.shortName = "plusmultiwave";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualPlusMultiWaveFactory::create(QWidget *parent)
{
    return new PlusMultiWave(parent);
}

Q_EXPORT_PLUGIN2(plusmultiwave,VisualPlusMultiWaveFactory)
