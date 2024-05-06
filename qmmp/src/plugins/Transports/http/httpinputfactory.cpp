#include "httpinputfactory.h"
#include "httpinputsource.h"
#include "settingsdialog.h"

InputSourceProperties HttpInputFactory::properties() const
{
    InputSourceProperties properties;
    properties.name = tr("Http Plugin");
    properties.shortName = "http";
    properties.protocols << "http" << "https";
    properties.hasSettings = true;
    return properties;
}

InputSource *HttpInputFactory::create(const QString &url, QObject *parent)
{
    return new HttpInputSource(url, parent);
}

QDialog *HttpInputFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(http, HttpInputFactory)
#endif
