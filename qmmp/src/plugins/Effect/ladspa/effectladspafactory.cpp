#include "effectladspafactory.h"
#include "settingsdialog.h"
#include "ladspahelper.h"

EffectProperties EffectLADSPAFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("LADSPA Plugin");
    properties.shortName = "ladspa";
    properties.hasSettings = true;
    return properties;
}

Effect *EffectLADSPAFactory::create()
{
    return new LADSPAHelper();
}

QDialog *EffectLADSPAFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ladspa, EffectLADSPAFactory)
#endif
