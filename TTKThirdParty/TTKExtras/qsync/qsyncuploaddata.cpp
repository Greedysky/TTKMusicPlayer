#include "qsyncuploaddata.h"
#include "qsyncdatainterface_p.h"

#include <QFile>

/*! @brief The class of the sync cloud upload data private.
 * @author Greedysky <greedysky@163.com>
 */
class QSyncUploadDataPrivate : public QSyncDataInterfacePrivate
{
public:
    QSyncUploadDataPrivate()
        : QSyncDataInterfacePrivate()
    {
    }

    QString m_uploadTime;
};



QSyncUploadData::QSyncUploadData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(TTK_CREATE_PRIVATE(QSyncUploadData), networkManager, parent)
{
    TTK_D(QSyncUploadData);
    d->m_manager = networkManager;
}

void QSyncUploadData::request(const QString &time, const QString &bucket, const QString &fileName, const QString &filePath)
{
    TTK_D(QSyncUploadData);
    d->m_uploadTime = time;

    const QString &method = "PUT";
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

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll();
    file.close();

    QNetworkReply *reply = d->m_manager->put(request, fileData);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);

    if(parent()->metaObject()->indexOfSlot("uploadProgress(QString,qint64,qint64)") != -1)
    {
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(uploadProgress(qint64,qint64)));
        connect(this, SIGNAL(uploadProgressChanged(QString,qint64,qint64)), parent(), SLOT(uploadProgress(QString,qint64,qint64)));
    }
}

void QSyncUploadData::receiveDataFromServer()
{
    TTK_D(QSyncUploadData);
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
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

void QSyncUploadData::uploadProgress(qint64 percent, qint64 total)
{
    TTK_D(QSyncUploadData);
    Q_EMIT uploadProgressChanged(d->m_uploadTime, percent, total);
}
