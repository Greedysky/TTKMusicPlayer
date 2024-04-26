#include "effectsonicfactory.h"
#include "sonicplugin.h"
#include "settingsdialog.h"

EffectProperties EffectSonicFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Speed Pitch Plugin");
    properties.shortName = "sonic";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectSonicFactory::create()
{
    return new SonicPlugin();
}

void EffectSonicFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sonic, EffectSonicFactory)
#endif
