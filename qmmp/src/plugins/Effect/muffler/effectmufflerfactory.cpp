#include "effectmufflerfactory.h"
#include "mufflerplugin.h"
#include "settingsdialog.h"

EffectProperties EffectMufflerFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Voice Removal Plugin");
    properties.shortName = "muffler";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectMufflerFactory::create()
{
    return new MufflerPlugin();
}

QDialog *EffectMufflerFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(muffler, EffectMufflerFactory)
#endif
