#include "effectcrystalizerfactory.h"
#include "crystalizerplugin.h"
#include "settingsdialog.h"

EffectProperties EffectCrystalizerFactory::properties() const
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

QDialog *EffectCrystalizerFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(crystalizer, EffectCrystalizerFactory)
#endif
