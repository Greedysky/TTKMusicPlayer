#include "musicnetworkoperator.h"
#include "musicdownloadsourcerequest.h"

#define IP_CHECK_URL    "ZlhkcnFzd1RabVhCZXNWM1pnbk5hT1ErL2RpMUNjK0hYQ3FXUHdCOVNGSlpDU2ZmNTZnekhHUlo3WkwrUWhtQXljNitUcjJmZ0RId004OFc5QlVibjhvRGlRSzY3QXlVbmZHNFV3bkhZdGZMU2JTZ3lJTkNhOGZJUlNhcmlBUmcvRUVrQWc9PQ=="

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

void MusicNetworkOperator::startRequest()
{
    MusicDownloadSourceRequest *d = new MusicDownloadSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startRequest(MusicUtils::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &bytes)
{
    QString line;
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
    }
    else
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("result"))
            {
                value = value["result"].toMap();
                line = value["operators"].toString();
            }
        }
    }

    Q_EMIT queryNetworkOperatorFinished(line);
    deleteLater();
}
