#include "settingsdialog.h"
#include "ladspahelper.h"
#include "effectladspafactory.h"

const EffectProperties EffectLADSPAFactory::properties() const
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

void EffectLADSPAFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ladspa, EffectLADSPAFactory)
#endif
