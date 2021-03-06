#include "outputalsa.h"
#include "outputalsafactory.h"
#include "settingsdialog.h"

OutputProperties OutputALSAFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("ALSA Plugin");
    properties.shortName = "alsa";
    properties.hasSettings = true;
    return properties;
}

Output* OutputALSAFactory::create()
{
    return new OutputALSA();
}

Volume *OutputALSAFactory::createVolume()
{
    return new VolumeALSA();
}

void OutputALSAFactory::showSettings(QWidget *parent)
{
    SettingsDialog *s = new SettingsDialog(parent);
    s->show();
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alsa, OutputALSAFactory)
#endif
