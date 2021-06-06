#include "visualwavecrestfactory.h"
#include "wavecrest.h"

VisualProperties VisualWaveCrestFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Wave Crest Plugin");
    properties.shortName = "wavecrest";
    return properties;
}

Visual *VisualWaveCrestFactory::create(QWidget *parent)
{
    return new WaveCrest(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wavecrest, VisualWaveCrestFactory)
#endif
