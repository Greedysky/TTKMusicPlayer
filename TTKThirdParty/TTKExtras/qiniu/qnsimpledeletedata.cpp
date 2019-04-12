#include "qnsimpledeletedata.h"
#include "qniohelper.h"
#include "qnmac.h"

#include <QDebug>

class QNSimpleDeleteDataPrivate : public TTKPrivate<QNSimpleDeleteData>
{
public:
    QNSimpleDeleteDataPrivate();

    QNetworkAccessManager *m_networkManager;

};

QNSimpleDeleteDataPrivate::QNSimpleDeleteDataPrivate()
{

}



QNSimpleDeleteData::QNSimpleDeleteData(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(QNSimpleDeleteData);
    d->m_networkManager = networkManager;
}

void QNSimpleDeleteData::deleteDataToServer(const QString &bucket, const QString &key)
{
    TTK_D(QNSimpleDeleteData);
    const QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNetworkRequest request = QNIOHelper::deleteRequest(bucket, key, &mac);
    QNetworkReply *reply = d->m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
}

void QNSimpleDeleteData::receiveDataFromServer()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
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
