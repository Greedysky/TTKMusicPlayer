#include "qsyncuploaddata.h"
#include "qsyncdatainterface_p.h"

#include <QFile>

class QSyncUploadDataPrivate : public QSyncDataInterfacePrivate
{
public:
    QSyncUploadDataPrivate() : QSyncDataInterfacePrivate()
    {
    }

    QString m_uploadTime;
};



QSyncUploadData::QSyncUploadData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{
    TTK_INIT_PUBLIC(QSyncUploadData);
    TTK_INIT_PRIVATE;
    TTK_D(QSyncUploadData);
    d->m_manager = networkManager;
}

void QSyncUploadData::uploadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    TTK_D(QSyncUploadData);
    d->m_uploadTime = time;

    const QString &method = "PUT";
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

    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QByteArray fileData = file.readAll();
    file.close();

    QNetworkReply *reply = d->m_manager->put(request, fileData);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
    if(parent()->metaObject()->indexOfSlot("uploadProgress(QString,qint64,qint64)") != -1)
    {
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(uploadProgress(qint64,qint64)));
        connect(this, SIGNAL(uploadProgressChanged(QString,qint64,qint64)), parent(), SLOT(uploadProgress(QString,qint64,qint64)));
    }
}

void QSyncUploadData::receiveDataFromServer()
{
    TTK_D(QSyncUploadData);
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            Q_EMIT uploadFileFinished(d->m_uploadTime);
        }
        else
        {
            Q_EMIT uploadFileFinished(TTK_NAN_STR);
        }
        reply->deleteLater();
    }
    else
    {
        Q_EMIT uploadFileFinished(TTK_NAN_STR);
    }
}

void QSyncUploadData::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    TTK_D(QSyncUploadData);
    Q_EMIT uploadProgressChanged(d->m_uploadTime, bytesSent, bytesTotal);
}
