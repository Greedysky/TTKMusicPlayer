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

QDialog *EffectStereoFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(stereo, EffectStereoFactory)
#endif
