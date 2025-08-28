#include "musicnetworkoperator.h"
#include "musicdatasourcerequest.h"

static constexpr const char *IP_CHECK_URL = "ZlhkcnFzd1RabVhCZXNWM1pnbk5hT1ErL2RpMUNjK0hYQ3FXUHdCOVNGSlpDU2ZmNTZnekhHUlo3WkwrUWhtQXljNitUcjJmZ0RId004OFc5QlVibjhvRGlRSzY3QXlVbmZHNFV3bkhZdGZMU2JTZ3lJTkNhOGZJUlNhcmlBUmcvRUVrQWc9PQ==";

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

void MusicNetworkOperator::startToRequest()
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    req->startToRequest(TTK::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &bytes)
{
    QString line;
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
    }
    else
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
