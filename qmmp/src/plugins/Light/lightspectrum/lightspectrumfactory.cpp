#include "lightspectrumfactory.h"
#include "spek-spectrogram.h"

const LightProperties LightSpectrumFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light Spectrum Plugin");
    properties.shortName = "lightspectrum";
    return properties;
}

Light *LightSpectrumFactory::create(QWidget *parent)
{
    return new LightSpectrum(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightspectrum, LightSpectrumFactory)
#endif
