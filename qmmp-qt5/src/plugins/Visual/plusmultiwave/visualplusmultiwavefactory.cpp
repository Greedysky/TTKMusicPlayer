#include <qmmp/qmmp.h>
#include "visualplusmultiwavefactory.h"
#include "plusmultiwave.h"

const VisualProperties VisualPlusMultiWaveFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Plus MultiWave Plugin");
    properties.shortName = "plusmultiwave";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualPlusMultiWaveFactory::create(QWidget *parent)
{
    return new PlusMultiWave(parent);
}
