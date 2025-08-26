#include "qsyncdatainterface.h"
#include "qsyncdatainterface_p.h"

QSyncDataInterface::QSyncDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(TTK_CREATE_PRIVATE(QSyncDataInterface), networkManager, parent)
{

}

QSyncDataInterface::QSyncDataInterface(QSyncDataInterfacePrivate &pvt, QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE_OBJECT(pvt);
    TTK_D(QSyncDataInterface);
    d->m_manager = networkManager;
}

void QSyncDataInterface::replyError(QNetworkReply::NetworkError error)
{
    TTK_ERROR_STREAM("Sync network error, code is " << error);
}

QString QSyncDataInterface::urlPathEncode(const QString &data) const
{
    return QUrl(data).toEncoded();
}
