#include "qnsimpleuploaddata.h"
#include "qnconf.h"
#include "qniohelper.h"
#include "qnputpolicy.h"
#include "qnmac.h"
#include "qnputret.h"

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
            //get http status code
            int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            //do some error management
            qDebug() << httpStatus;
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
