#include "effectmixerfactory.h"
#include "mixerplugin.h"
#include "settingsdialog.h"

EffectProperties EffectMixerFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Channel Mixer Plugin");
    properties.shortName = "mixer";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectMixerFactory::create()
{
    return new MixerPlugin();
}

void EffectMixerFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mixer, EffectMixerFactory)
#endif
