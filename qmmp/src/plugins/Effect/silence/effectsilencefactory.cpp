#include "effectsilencefactory.h"
#include "silenceplugin.h"
#include "settingsdialog.h"

EffectProperties EffectSilenceFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Silence Removal Plugin");
    properties.shortName = "silence";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectSilenceFactory::create()
{
    return new SilencePlugin();
}

QDialog *EffectSilenceFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(silence, EffectSilenceFactory)
#endif
