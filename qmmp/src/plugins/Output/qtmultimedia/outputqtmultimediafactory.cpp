#include "outputqtmultimediafactory.h"
#include "outputqtmultimedia.h"
#include "settingsdialog.h"

OutputProperties OutputQtMultimediaFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("Qt Multimedia Plugin");
    properties.shortName = "qtmultimedia";
    properties.hasSettings = true;
    return properties;
}

Output *OutputQtMultimediaFactory::create()
{
    return new OutputQtMultimedia();
}

Volume *OutputQtMultimediaFactory::createVolume()
{
    return nullptr;
}

QDialog *OutputQtMultimediaFactory::createSettings(QWidget* parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(qtmultimedia, OutputQtMultimediaFactory)
#endif
