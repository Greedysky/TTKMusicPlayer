#include <qmmp/qmmp.h>
#include "visualspectrumspekfactory.h"
#include "spek-spectrogram.h"

const SpekProperties VisualSpectrumSpekFactory::properties() const
{
    SpekProperties properties;
    properties.name = tr("Spectrum Spek Plugin");
    properties.shortName = "spectrumspek";
    properties.hasSettings = false;
    properties.hasAbout = false;
    return properties;
}

Spek *VisualSpectrumSpekFactory::create(QWidget *parent)
{
    return new SpectrumSpek(parent);
}
