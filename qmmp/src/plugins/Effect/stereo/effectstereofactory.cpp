#include "effectstereofactory.h"
#include "settingsdialog.h"
#include "stereoplugin.h"

const EffectProperties EffectStereoFactory::properties() const
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
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(stereo, EffectStereoFactory)
#endif
