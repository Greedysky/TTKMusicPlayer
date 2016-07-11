#include "qnsimpleuploaddata.h"
#include "qnconf.h"
#include "qniohelper.h"
#include "qnputpolicy.h"
#include "qnmac.h"
#include "qnputret.h"
#include "qnutils.h"

QNSimpleUploadData::QNSimpleUploadData(QNetworkAccessManager *networkManager,
                                       QObject *parent)
    : QObject(parent)
{
    m_networkManager = networkManager;
}

void QNSimpleUploadData::uploadDataToServer(const QByteArray &data, const QString &bucket,
                                            const QString &key, const QString &name)
{
    QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNPutPolicy policy(bucket);
    QString uploadToken = policy.makeUploadToken(&mac);
    QHttpMultiPart *multiPart = QNIOHelper::createPutDataMultiPart(uploadToken, data, key, name);
    QNetworkRequest request;
    request.setUrl(QUrl(QNConf::UPLOAD_HOST));
    QNetworkReply *reply = m_networkManager->post(request, multiPart);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                   SLOT(handleError(QNetworkReply::NetworkError)));
}

QString QNSimpleUploadData::getDownloadUrl(const QString &domain, const QString &key)
{
    QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    int deadline = QNUtils::expireInSeconds(3600);
    QString baseUrl = QString("%1/%2?e=%3").arg(domain).arg(key).arg(deadline);

    QNPutPolicy policy(baseUrl);
    return QString("%1&token=%2").arg(baseUrl).arg(policy.makeDownloadToken(&mac));
}

void QNSimpleUploadData::receiveDataFromServer()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            QByteArray respData=reply->readAll();
            QNPutRet *putRet = QNPutRet::fromJSON(respData);
            emit uploadFileFinished(putRet->getKey());
            delete putRet;
        }
        else
        {
            emit uploadFileFinished( QString() );
        }
        reply->deleteLater();
    }
    else
    {
        emit uploadFileFinished( QString() );
    }
}

void QNSimpleUploadData::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
}
