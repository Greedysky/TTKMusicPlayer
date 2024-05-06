#include "outputossfactory.h"
#include "outputoss.h"
#include "settingsdialog.h"

OutputProperties OutputOSSFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("OSS Plugin");
    properties.shortName = "oss";
    properties.hasSettings = true;
    return properties;
}

Output* OutputOSSFactory::create()
{
    return new OutputOSS();
}

Volume *OutputOSSFactory::createVolume()
{
    return new VolumeOSS();
}

QDialog *OutputOSSFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(oss, OutputOSSFactory)
#endif
