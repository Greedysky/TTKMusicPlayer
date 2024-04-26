#include "lightspectrogramfactory.h"
#include "spek-spectrogram.h"

LightProperties LightSpectrogramFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light Spectrogram Plugin");
    properties.shortName = "lightspectrogram";
    return properties;
}

Light *LightSpectrogramFactory::create(QWidget *parent)
{
    return new LightSpectrogram(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightspectrogram, LightSpectrogramFactory)
#endif
