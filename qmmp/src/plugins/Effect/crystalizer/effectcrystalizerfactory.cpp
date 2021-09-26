#include "effectcrystalizerfactory.h"
#include "settingsdialog.h"
#include "crystalizerplugin.h"

const EffectProperties EffectCrystalizerFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Crystalizer Plugin");
    properties.shortName = "crystalizer";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectCrystalizerFactory::create()
{
    return new CrystalizerPlugin();
}

void EffectCrystalizerFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(crystalizer, EffectCrystalizerFactory)
#endif
