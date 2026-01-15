#include "mmsinputfactory.h"
#include "mmsinputsource.h"
#include "settingsdialog.h"

InputSourceProperties MMSInputFactory::properties() const
{
    InputSourceProperties properties;
    properties.name = tr("MMS Plugin");
    properties.shortName = "mms";
    properties.protocols << "mms" << "mmsh" << "mmsu" << "mmst";
    properties.hasSettings = true;
    return properties;
}

InputSource *MMSInputFactory::create(const QString &url, QObject *parent)
{
    return new MMSInputSource(url, parent);
}

QDialog *MMSInputFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mms, MMSInputFactory)
#endif
