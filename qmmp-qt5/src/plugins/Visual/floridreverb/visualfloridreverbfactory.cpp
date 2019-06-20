#include <qmmp/qmmp.h>
#include "visualfloridreverbfactory.h"
#include "floridreverb.h"

VisualProperties VisualFloridReverbFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Reverb Plugin");
    properties.shortName = "floridreverb";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualFloridReverbFactory::create(QWidget *parent)
{
    return new FloridReverb(parent);
}
