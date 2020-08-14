#include <bs2b/bs2bversion.h>
#include "effectbs2bfactory.h"
#include "settingsdialog.h"
#include "bs2bplugin.h"

const EffectProperties EffectBs2bFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("BS2B Plugin");
    properties.shortName = "bs2b";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectBs2bFactory::create()
{
    return new Bs2bPlugin();
}

void EffectBs2bFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bs2b, EffectBs2bFactory)
#endif
