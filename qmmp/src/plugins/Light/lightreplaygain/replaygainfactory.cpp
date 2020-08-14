#include "replaygainfactory.h"
#include "replaygainwidget.h"

const LightProperties LightReplaygGainFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light ReplayGain Plugin");
    properties.shortName = "lightreplaygain";
    properties.hasSettings = false;
    return properties;
}

Light *LightReplaygGainFactory::create(QWidget *parent)
{
    return new ReplayGainWidget(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightreplaygain, LightReplaygGainFactory)
#endif
