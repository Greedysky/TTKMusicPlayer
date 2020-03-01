#include "qossdatainterface.h"
#include "qossdatainterface_p.h"

QOSSDataInterface::QOSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(QOSSDataInterface);
    d->m_manager = networkManager;
}

void QOSSDataInterface::handleError(QNetworkReply::NetworkError error)
{
    TTK_LOGGER_ERROR("error" << error);
}

QString QOSSDataInterface::pathEncode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString(QUrl::FullyEncoded);
#else
    return QUrl(data).toEncoded();
#endif
}

QString QOSSDataInterface::pathDecode(const QString &data) const
{
#ifdef TTK_GREATER_NEW
    return QUrl(data).toString();
#else
    return QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}
