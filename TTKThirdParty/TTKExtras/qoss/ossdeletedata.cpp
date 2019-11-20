#include "ossdeletedata.h"
#include "ossdatainterface_p.h"

OSSDeleteData::OSSDeleteData(QNetworkAccessManager *networkManager, QObject *parent)
    : OSSDataInterface(networkManager, parent)
{

}

void OSSDeleteData::deleteDataOperator(const QString &bucket, const QString &file)
{
    TTK_D(OSSDataInterface);
    const QString &method = "DELETE";
    const QString &url = "/" + file;
    const QString &resource = "/" + bucket + "/" + file;
    const QString &host = bucket + "." + OSSConf::OSS_HOST;

    MStringMap headers;
    headers.insert("Date", OSSUtils::getGMT());
    headers.insert("Host", host);

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + host + url));

    MStringMapterator it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_networkManager->deleteResource(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
}

void OSSDeleteData::receiveDataFromServer()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        emit deleteFileFinished(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 204);
        reply->deleteLater();
    }
    else
    {
        emit deleteFileFinished(false);
    }
}
