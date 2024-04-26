#include "lightwaveformfactory.h"
#include "lightwaveform.h"

LightProperties LightWaveFormFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light WaveForm Plugin");
    properties.shortName = "lightwaveform";
    return properties;
}

Light *LightWaveFormFactory::create(QWidget *parent)
{
    return new LightWaveForm(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightwaveform, LightWaveFormFactory)
#endif
