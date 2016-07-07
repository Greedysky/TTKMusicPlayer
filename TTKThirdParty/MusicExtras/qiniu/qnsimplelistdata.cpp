#include "qnsimplelistdata.h"
#include "qnmac.h"
#include "qniohelper.h"

QNSimpleListData::QNSimpleListData(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{

}

void QNSimpleListData::listDataToServer(const QString &bucket)
{
    QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNetworkRequest request = QNIOHelper::listRequest(bucket, &mac);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                   SLOT(handleError(QNetworkReply::NetworkError)));
}

void QNSimpleListData::receiveDataFromServer()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        QByteArray respData = reply->readAll();
        int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << respData;
        qDebug() << httpStatusCode;
        reply->deleteLater();
    }
}

void QNSimpleListData::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
}
