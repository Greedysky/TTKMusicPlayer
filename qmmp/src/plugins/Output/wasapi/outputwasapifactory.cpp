#include "outputwasapifactory.h"
#include "outputwasapi.h"
#include "settingsdialog.h"

OutputProperties OutputWASAPIFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("WASAPI Plugin");
    properties.hasSettings = true;
    properties.shortName = "wasapi";
    return properties;
}

Output *OutputWASAPIFactory::create()
{
    return new OutputWASAPI();
}

Volume *OutputWASAPIFactory::createVolume()
{
    return new VolumeWASAPI();
}

QDialog *OutputWASAPIFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wasapi, OutputWASAPIFactory)
#endif
