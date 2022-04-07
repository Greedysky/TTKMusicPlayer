#include "qsyncdeletedata.h"
#include "qsyncdatainterface_p.h"

QSyncDeleteData::QSyncDeleteData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{

}

void QSyncDeleteData::deleteDataOperator(const QString &bucket, const QString &fileName)
{
    TTK_D(QSyncDataInterface);
    const QString &method = "DELETE";
    const QString &url = TTK_SEPARATOR + fileName;
    const QString &resource = TTK_SEPARATOR + bucket + url;
    const QString &host = bucket + TTK_DOT + QSyncConfig::HOST;

    TTKStringMap headers;
    headers.insert("Date", QSyncUtils::GMT());
    headers.insert("Host", host);
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl("http://" + host + url);

    TTKStringMapIter it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->deleteResource(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void QSyncDeleteData::receiveDataFromServer()
{
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        Q_EMIT deleteFileFinished(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 204);
        reply->deleteLater();
    }
    else
    {
        Q_EMIT deleteFileFinished(false);
    }
}
