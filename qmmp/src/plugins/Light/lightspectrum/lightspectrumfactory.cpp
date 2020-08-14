#include "lightspectrumfactory.h"
#include "spek-spectrogram.h"

const LightProperties LightSpectrumFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light Spectrum Plugin");
    properties.shortName = "lightspectrum";
    properties.hasSettings = false;
    return properties;
}

Light *LightSpectrumFactory::create(QWidget *parent)
{
    return new LightSpectrum(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightspectrum, LightSpectrumFactory)
#endif
