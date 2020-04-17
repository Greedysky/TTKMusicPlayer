#include "qsyncdatainterface.h"
#include "qsyncdatainterface_p.h"

QSyncDataInterface::QSyncDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(QSyncDataInterface);
    d->m_manager = networkManager;
}

void QSyncDataInterface::handleError(QNetworkReply::NetworkError error)
{
    TTK_LOGGER_ERROR("error" << error);
}

QString QSyncDataInterface::pathEncode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString(QUrl::FullyEncoded);
#else
    return QUrl(data).toEncoded();
#endif
}

QString QSyncDataInterface::pathDecode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString();
#else
    return QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}
