#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visuallightspectrumfactory.h"
#include "spek-spectrogram.h"

const LightProperties VisualLightSpectrumFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light Spectrum Plugin");
    properties.shortName = "lightspectrum";
    properties.hasSettings = false;
    return properties;
}

Light *VisualLightSpectrumFactory::create(QWidget *parent)
{
    return new LightSpectrum(parent);
}

Q_EXPORT_PLUGIN2(lightspectrum,VisualLightSpectrumFactory)
