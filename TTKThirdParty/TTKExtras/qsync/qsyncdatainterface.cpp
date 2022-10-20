#include "qsyncdatainterface.h"
#include "qsyncdatainterface_p.h"

QSyncDataInterface::QSyncDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(TTK_CREATE_PRIVATE(QSyncDataInterface), networkManager, parent)
{

}

QSyncDataInterface::QSyncDataInterface(QSyncDataInterfacePrivate &pvt, QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE_D(pvt);
    TTK_D(QSyncDataInterface);
    d->m_manager = networkManager;
}

void QSyncDataInterface::replyError(QNetworkReply::NetworkError error)
{
    TTK_ERROR_STREAM("Sync network error" << error);
}

QString QSyncDataInterface::pathEncode(const QString &data) const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return QUrl(data).toString(QUrl::FullyEncoded);
#else
    return QUrl(data).toEncoded();
#endif
}

QString QSyncDataInterface::pathDecode(const QString &data) const
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    return QUrl(data).toString();
#else
    return QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}
