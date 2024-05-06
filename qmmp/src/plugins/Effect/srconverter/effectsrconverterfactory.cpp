#include "effectsrconverterfactory.h"
#include "settingsdialog.h"
#include "srconverter.h"

EffectProperties EffectSRConverterFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("SRC Plugin");
    properties.shortName = "srconverter";
    properties.hasSettings = true;
    properties.priority = EffectProperties::EFFECT_PRIORITY_HIGH;
    return properties;
}

Effect *EffectSRConverterFactory::create()
{
    return new SRConverter();
}

QDialog *EffectSRConverterFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(srconverter, EffectSRConverterFactory)
#endif
