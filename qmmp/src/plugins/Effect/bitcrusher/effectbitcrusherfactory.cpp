#include "effectbitcrusherfactory.h"
#include "bitcrusherplugin.h"
#include "settingsdialog.h"

EffectProperties EffectBitcrusherFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Bitcrusher Plugin");
    properties.shortName = "bitcrusher";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectBitcrusherFactory::create()
{
    return new BitcrusherPlugin();
}

void EffectBitcrusherFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bitcrusher, EffectBitcrusherFactory)
#endif
