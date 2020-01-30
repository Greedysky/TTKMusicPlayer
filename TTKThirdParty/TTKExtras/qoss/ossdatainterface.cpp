#include "ossdatainterface.h"
#include "ossdatainterface_p.h"

OSSDataInterface::OSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(OSSDataInterface);
    d->m_networkManager = networkManager;
}

void OSSDataInterface::handleError(QNetworkReply::NetworkError error)
{
    M_LOGGER_ERROR("error" << error);
}

QString OSSDataInterface::pathEncode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString(QUrl::FullyEncoded);
#else
    return QUrl(data).toEncoded();
#endif
}

QString OSSDataInterface::pathDecode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString();
#else
    return QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}
