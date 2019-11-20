#include "ossdownloaddata.h"
#include "ossdatainterface_p.h"

#include <QFile>
#include <QDateTime>

class OSSDownloadDataPrivate : public OSSDataInterfacePrivate
{
public:
    OSSDownloadDataPrivate() : OSSDataInterfacePrivate()
    {
    }

    QString m_downloadTime;
    QString m_downloadPath;
};



OSSDownloadData::OSSDownloadData(QNetworkAccessManager *networkManager, QObject *parent)
    : OSSDataInterface(networkManager, parent)
{
    TTK_INIT_PRIVATE;
}

void OSSDownloadData::downloadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    TTK_D(OSSDownloadData);
    d->m_downloadTime = time;
    d->m_downloadPath = filePath;

    const QString &method = "GET";
    const QString &url = "/" + fileName;
    const QString &resource = "/" + bucket + "/" + fileName;
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

    QNetworkReply *reply = d->m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));

    if(parent()->metaObject()->indexOfSlot("downloadProgress(QString,qint64,qint64)") != -1)
    {
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
        connect(this, SIGNAL(downloadProgressChanged(QString,qint64,qint64)), parent(), SLOT(downloadProgress(QString,qint64,qint64)));
    }
}

QString OSSDownloadData::getDownloadUrl(const QString &bucket, const QString &fileName)
{
    return getDownloadUrl(bucket, fileName, "application/x-www-form-urlencoded");
}

QString OSSDownloadData::getDownloadUrl(const QString &bucket, const QString &fileName, const QString &contentType)
{
    const qint64 deadline = QDateTime::currentDateTime().toUTC().addSecs(60*30).toMSecsSinceEpoch() / 1000;
#ifdef TTK_GREATER_NEW
    const QString encodeKey = QUrl(fileName).toString(QUrl::FullyEncoded);
#else
    const QString encodeKey = QUrl(fileName).toEncoded();
#endif
    const QString &method = "GET";
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + OSSConf::OSS_HOST;

    MStringMap headers;
    headers.insert("Date", QString::number(deadline));
    headers.insert("Content-Type", contentType);
    headers.insert("Host", host);

    const QString &signature = OSSUtils::getAuthorizationCode(OSSConf::SECRET_KEY, method, headers, resource);
    return QString("http://%1/%2?OSSAccessKeyId=%3&Expires=%4&Signature=%5").arg(host).arg(encodeKey).arg(OSSConf::ACCESS_KEY).arg(deadline).arg(signature);
}

void OSSDownloadData::receiveDataFromServer()
{
    TTK_D(OSSDownloadData);
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            QFile file(d->m_downloadPath);
            if(file.open(QFile::WriteOnly))
            {
                file.write(reply->readAll());
                file.close();
            }
            emit downloadFileFinished(d->m_downloadTime);
        }
        else
        {
            emit downloadFileFinished(TTK_ERROR_STR);
        }
        reply->deleteLater();
    }
    else
    {
        emit downloadFileFinished(TTK_ERROR_STR);
    }
}

void OSSDownloadData::downloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    TTK_D(OSSDownloadData);
    emit downloadProgressChanged(d->m_downloadTime, bytesSent, bytesTotal);
}
