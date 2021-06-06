#include "visualfloridreverbfactory.h"
#include "floridreverb.h"

VisualProperties VisualFloridReverbFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Reverb Plugin");
    properties.shortName = "floridreverb";
    return properties;
}

Visual *VisualFloridReverbFactory::create(QWidget *parent)
{
    return new FloridReverb(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridreverb, VisualFloridReverbFactory)
#endif
