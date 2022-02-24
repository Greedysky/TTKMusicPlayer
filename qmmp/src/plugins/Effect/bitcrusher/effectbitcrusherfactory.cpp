#include "effectbitcrusherfactory.h"
#include "bitcrusherplugin.h"
#include "settingsdialog.h"

const EffectProperties EffectBitcrusherFactory::properties() const
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
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bitcrusher, EffectBitcrusherFactory)
#endif
