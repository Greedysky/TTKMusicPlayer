#include "ossdownloaddata.h"

#include <QFile>
#include <QDateTime>

OSSDownloadData::OSSDownloadData(QNetworkAccessManager *networkManager, QObject *parent)
    : OSSDataInterface(networkManager, parent)
{

}

void OSSDownloadData::downloadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    m_downloadTime = time;
    m_downloadPath = filePath;

    const QString &method = "GET";
    const QString &url = "/" + fileName;
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + OSSConf::OSS_HOST;

    MStringMap headers;
    headers.insert("Date", OSSUtils::getGMT());
    headers.insert("Host", host);

    insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + host + url));

    MStringMapterator it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
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
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            QFile file(m_downloadPath);
            if(file.open(QFile::WriteOnly))
            {
                file.write(reply->readAll());
                file.close();
            }
            emit downloadFileFinished(m_downloadTime);
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
    emit downloadProgressChanged(m_downloadTime, bytesSent, bytesTotal);
}
