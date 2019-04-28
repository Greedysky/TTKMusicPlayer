#include "qnsimpleuploaddata.h"
#include "qnconf.h"
#include "qniohelper.h"
#include "qnputpolicy.h"
#include "qnmac.h"
#include "qnputret.h"
#include "qnutils.h"

#include <QUrl>
#include <QDebug>
#include <QStringList>

class QNSimpleUploadDataPrivate : public TTKPrivate<QNSimpleUploadData>
{
public:
    QNSimpleUploadDataPrivate();

    QString m_uploadTime;
    QNetworkAccessManager *m_networkManager;

};

QNSimpleUploadDataPrivate::QNSimpleUploadDataPrivate()
{

}



QNSimpleUploadData::QNSimpleUploadData(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(QNSimpleUploadData);
    d->m_networkManager = networkManager;
}

void QNSimpleUploadData::uploadDataToServer(const QString &time, const QByteArray &data, const QString &bucket, const QString &key, const QString &name)
{
    TTK_D(QNSimpleUploadData);
    d->m_uploadTime = time;

    const QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNPutPolicy policy(bucket);
    const QString &uploadToken = policy.makeUploadToken(&mac);
    QHttpMultiPart *multiPart = QNIOHelper::createPutDataMultiPart(uploadToken, data, key, name);
    QNetworkRequest request;
    request.setUrl(QUrl(QNConf::UPLOAD_HOST));

    QNetworkReply *reply = d->m_networkManager->post(request, multiPart);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
    if(parent()->metaObject()->indexOfSlot("uploadProgress(QString,qint64,qint64)") != -1)
    {
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(uploadProgress(qint64,qint64)));
        connect(this, SIGNAL(uploadProgressChanged(QString,qint64,qint64)), parent(), SLOT(uploadProgress(QString,qint64,qint64)));
    }
}

QString QNSimpleUploadData::getDownloadUrl(const QString &domain, const QString &key)
{
    const QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    const int deadline = QNUtils::expireInSeconds(3600);
#ifdef TTK_GREATER_NEW
    const QString encodeKey = QUrl(key).toString(QUrl::FullyEncoded);
#else
    const QString encodeKey = QUrl(key).toEncoded();
#endif
    const QString &baseUrl = QString("%1/%2?e=%3").arg(domain).arg(encodeKey).arg(deadline);

    QNPutPolicy policy(baseUrl);
    return QString("%1&token=%2").arg(baseUrl).arg(policy.makeDownloadToken(&mac));
}

void QNSimpleUploadData::receiveDataFromServer()
{
    TTK_D(QNSimpleUploadData);
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            emit uploadFileFinished(d->m_uploadTime);
        }
        else
        {
            emit uploadFileFinished(TTK_ERROR_STR);
        }
        reply->deleteLater();
    }
    else
    {
        emit uploadFileFinished(TTK_ERROR_STR);
    }
}

void QNSimpleUploadData::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
}

void QNSimpleUploadData::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    TTK_D(QNSimpleUploadData);
    emit uploadProgressChanged(d->m_uploadTime, bytesSent, bytesTotal);
}
