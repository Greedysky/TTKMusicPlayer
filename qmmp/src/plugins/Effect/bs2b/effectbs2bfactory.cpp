#include "effectbs2bfactory.h"
#include "bs2bplugin.h"
#include "settingsdialog.h"

EffectProperties EffectBs2bFactory::properties() const
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
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bs2b, EffectBs2bFactory)
#endif
