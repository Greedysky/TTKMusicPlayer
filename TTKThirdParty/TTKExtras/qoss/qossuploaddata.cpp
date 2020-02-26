#include "qossuploaddata.h"
#include "qossdatainterface_p.h"

#include <QFile>

class QOSSUploadDataPrivate : public QOSSDataInterfacePrivate
{
public:
    QOSSUploadDataPrivate() : QOSSDataInterfacePrivate()
    {
    }

    QString m_uploadTime;
};



QOSSUploadData::QOSSUploadData(QNetworkAccessManager *networkManager, QObject *parent)
    : QOSSDataInterface(networkManager, parent)
{
    TTK_INIT_PUBLIC(QOSSUploadData);
    TTK_INIT_PRIVATE;
    TTK_D(QOSSUploadData);
    d->m_manager = networkManager;
}

void QOSSUploadData::uploadDataOperator(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    TTK_D(QOSSUploadData);
    d->m_uploadTime = time;

    const QString &method = "PUT";
    const QString &url = "/" + fileName;
    const QString &resource = "/" + bucket + "/" + fileName;
    const QString &host = bucket + "." + QOSSConf::OSS_HOST;

    TTKStringMap headers;
    headers.insert("Date", QOSSUtils::getGMT());
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

void QOSSUploadData::receiveDataFromServer()
{
    TTK_D(QOSSUploadData);
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

void QOSSUploadData::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    TTK_D(QOSSUploadData);
    Q_EMIT uploadProgressChanged(d->m_uploadTime, bytesSent, bytesTotal);
}
