#include "outputoss4factory.h"
#include "outputoss4.h"
#include "settingsdialog.h"

OutputProperties OutputOSS4Factory::properties() const
{
    OutputProperties properties;
    properties.name = tr("OSS4 Plugin");
    properties.shortName = "oss4";
    properties.hasSettings = true;
    return properties;
}

Output* OutputOSS4Factory::create()
{
    return new OutputOSS4();
}

Volume *OutputOSS4Factory::createVolume()
{
    return new VolumeOSS4();
}

void OutputOSS4Factory::showSettings(QWidget* parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(oss4, OutputOSS4Factory)
#endif
