#include "qnsimpledeletedata.h"
#include "qniohelper.h"
#include "qnmac.h"

#include <QDebug>

QNSimpleDeleteData::QNSimpleDeleteData(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{

}

void QNSimpleDeleteData::deleteDataToServer(const QString &bucket, const QString &key)
{
    QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNetworkRequest request = QNIOHelper::deleteRequest(bucket, key, &mac);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                   SLOT(handleError(QNetworkReply::NetworkError)));
}

void QNSimpleDeleteData::receiveDataFromServer()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        emit deleteFileFinished(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200);
        reply->deleteLater();
    }
    else
    {
        emit deleteFileFinished(false);
    }
}

void QNSimpleDeleteData::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
}
