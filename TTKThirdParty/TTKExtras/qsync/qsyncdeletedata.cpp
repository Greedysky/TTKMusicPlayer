#include "qsyncdeletedata.h"
#include "qsyncdatainterface_p.h"

QSyncDeleteData::QSyncDeleteData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{

}

void QSyncDeleteData::request(const QString &bucket, const QString &fileName)
{
    TTK_D(QSyncDataInterface);
    const QString &method = "DELETE";
    const QString &url = TTK_SEPARATOR + fileName;
    const QString &resource = TTK_SEPARATOR + bucket + url;
    const QString &host = bucket + TTK_DOT + QSyncConfig::HOST;

    TTKStringMap headers;
    headers.insert("Host", host);
    headers.insert("Date", QSyncUtils::GMT());
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(HTTP_PROTOCOL + host + url);

    for(auto it = headers.constBegin(); it != headers.constEnd(); ++it)
    {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->deleteResource(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void QSyncDeleteData::receiveDataFromServer()
{
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
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
