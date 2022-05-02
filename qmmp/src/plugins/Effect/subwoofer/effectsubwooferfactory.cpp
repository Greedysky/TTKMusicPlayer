#include "effectsubwooferfactory.h"
#include "subwooferplugin.h"
#include "settingsdialog.h"

const EffectProperties EffectSubwooferFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Subwoofer Simulator Plugin");
    properties.shortName = "subwoofer";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectSubwooferFactory::create()
{
    return new SubwooferPlugin();
}

void EffectSubwooferFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(subwoofer, EffectSubwooferFactory)
#endif
