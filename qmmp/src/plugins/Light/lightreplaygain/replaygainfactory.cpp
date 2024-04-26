#include "replaygainfactory.h"
#include "replaygainwidget.h"

LightProperties LightReplaygGainFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light ReplayGain Plugin");
    properties.shortName = "lightreplaygain";
    return properties;
}

Light *LightReplaygGainFactory::create(QWidget *parent)
{
    return new ReplayGainWidget(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightreplaygain, LightReplaygGainFactory)
#endif
