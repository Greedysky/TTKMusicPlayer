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

QDialog *EffectBitcrusherFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bitcrusher, EffectBitcrusherFactory)
#endif
