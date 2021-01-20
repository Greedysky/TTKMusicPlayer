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
    const QString &url = "/" + fileName;
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + QSyncConf::HOST;

    TTKStringMap headers;
    headers.insert("Date", QSyncUtils::getGMT());
    headers.insert("Host", host);
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl("http://" + host + url);

    TTKStringMapterator it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->deleteResource(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
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
