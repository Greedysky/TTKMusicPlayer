#include "effectsoxrfactory.h"
#include "soxresampler.h"
#include "settingsdialog.h"

EffectProperties EffectSoXRFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("SoX Resampler Plugin");
    properties.shortName = "soxr";
    properties.hasSettings = true;
    properties.priority = EffectProperties::EFFECT_PRIORITY_HIGH;
    return properties;
}

Effect *EffectSoXRFactory::create()
{
    return new SoXResampler();
}

void EffectSoXRFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(soxr, EffectSoXRFactory)
#endif
