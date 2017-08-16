#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualqspekfactory.h"
#include "spek-spectrogram.h"

const SpekProperties VisualQSpekFactory::properties() const
{
    SpekProperties properties;
    properties.name = tr("QSpek Plugin");
    properties.shortName = "qspek";
    properties.hasSettings = false;
    properties.hasAbout = false;
    return properties;
}

Spek *VisualQSpekFactory::create(QWidget *parent)
{
    return new SpekSpectrogram(parent);
}

Q_EXPORT_PLUGIN2(qspek,VisualQSpekFactory)

