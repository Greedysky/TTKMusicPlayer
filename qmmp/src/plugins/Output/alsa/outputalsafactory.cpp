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

Output *OutputALSAFactory::create()
{
    return new OutputALSA();
}

Volume *OutputALSAFactory::createVolume()
{
    return new VolumeALSA();
}

void OutputALSAFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alsa, OutputALSAFactory)
#endif
