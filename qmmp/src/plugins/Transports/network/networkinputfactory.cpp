#include "networkinputfactory.h"
#include "networkinputsource.h"

InputSourceProperties NetworkInputFactory::properties() const
{
    InputSourceProperties properties;
    properties.name = tr("Network Plugin");
    properties.shortName = "network";
    properties.protocols << "http" << "https";
    properties.hasSettings = true;
    return properties;
}

InputSource *NetworkInputFactory::create(const QString &url, QObject *parent)
{
    return new NetworkInputSource(url, parent);
}

void NetworkInputFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(network, NetworkInputFactory)
#endif
