#include <qmmp/qmmp.h>
#include "visualfloridreverberationfactory.h"
#include "floridreverberation.h"

VisualProperties VisualFloridReverberationFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Reverberation Plugin");
    properties.shortName = "floridreverberation";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridReverberationFactory::create(QWidget *parent)
{
    return new FloridReverberation(parent);
}
