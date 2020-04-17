#include "qsyncdownloaddata.h"
#include "qsyncdatainterface_p.h"

#include <QFile>
#include <QDateTime>

class QSyncDownloadDataPrivate : public QSyncDataInterfacePrivate
{
public:
    QSyncDownloadDataPrivate() : QSyncDataInterfacePrivate()
    {
    }

    QString m_downloadTime;
    QString m_downloadPath;
};



QSyncDownloadData::QSyncDownloadData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{
    TTK_INIT_PUBLIC(QSyncDownloadData);
    TTK_INIT_PRIVATE;
    TTK_D(QSyncDownloadData);
    d->m_manager = networkManager;
}

void QSyncDownloadData::downloadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    TTK_D(QSyncDownloadData);
    d->m_downloadTime = time;
    d->m_downloadPath = filePath;

    const QString &method = "GET";
    const QString &url = "/" + fileName;
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + QSyncConf::HOST;

    TTKStringMap headers;
    headers.insert("Date", QSyncUtils::getGMT());
    headers.insert("Host", host);
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + host + url));

    TTKStringMapterator it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));

    if(parent()->metaObject()->indexOfSlot("downloadProgress(QString,qint64,qint64)") != -1)
    {
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
        connect(this, SIGNAL(downloadProgressChanged(QString,qint64,qint64)), parent(), SLOT(downloadProgress(QString,qint64,qint64)));
    }
}

QString QSyncDownloadData::getDownloadUrl(const QString &bucket, const QString &fileName)
{
    return getDownloadUrl(bucket, fileName, "application/x-www-form-urlencoded");
}

QString QSyncDownloadData::getDownloadUrl(const QString &bucket, const QString &fileName, const QString &contentType)
{
    const qint64 deadline = QDateTime::currentDateTime().toUTC().addSecs(60 * 30).toMSecsSinceEpoch() / 1000;

    const QString &encodeKey = pathEncode(fileName);
    const QString &method = "GET";
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + QSyncConf::HOST;

    TTKStringMap headers;
    headers.insert("Date", QString::number(deadline));
    headers.insert("Content-Type", contentType);
    headers.insert("Host", host);

    const QString &signature = QSyncUtils::getAuthorizationCode(QSyncConf::KEY, method, headers, resource);
    return QString("http://%1/%2?OSSAccessKeyId=%3&Expires=%4&Signature=%5").arg(host).arg(encodeKey).arg(QSyncConf::NAME).arg(deadline).arg(signature);
}

void QSyncDownloadData::receiveDataFromServer()
{
    TTK_D(QSyncDownloadData);
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
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
            Q_EMIT downloadFileFinished(d->m_downloadTime);
        }
        else
        {
            Q_EMIT downloadFileFinished(TTK_NAN_STR);
        }
        reply->deleteLater();
    }
    else
    {
        Q_EMIT downloadFileFinished(TTK_NAN_STR);
    }
}

void QSyncDownloadData::downloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    TTK_D(QSyncDownloadData);
    Q_EMIT downloadProgressChanged(d->m_downloadTime, bytesSent, bytesTotal);
}
