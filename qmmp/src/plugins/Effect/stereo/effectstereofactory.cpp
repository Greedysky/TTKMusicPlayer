#include "effectstereofactory.h"
#include "stereoplugin.h"
#include "settingsdialog.h"

EffectProperties EffectStereoFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Extra Stereo Plugin");
    properties.shortName = "stereo";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectStereoFactory::create()
{
    return new StereoPlugin();
}

void EffectStereoFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(stereo, EffectStereoFactory)
#endif
