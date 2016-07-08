#include "qnsimplelistdata.h"
#include "qnmac.h"
#include "qniohelper.h"

#   include <QJsonParseError>
#   include <QJsonDocument>
#   include <QJsonObject>
#   include <QJsonArray>

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
        QNDataItems items;
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument parseDoucment = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            ///Put the data into Json
            if(jsonError.error != QJsonParseError::NoError ||
               !parseDoucment.isObject())
            {
                emit receiveFinshed( QNDataItems() );
                return ;
            }

            QJsonObject jsonObject = parseDoucment.object();
            if(jsonObject.contains("items"))
            {
                QJsonArray array = jsonObject.take("items").toArray();
                foreach(QJsonValue value, array)
                {
                    if(!value.isObject())
                    {
                       continue;
                    }
                    QJsonObject object = value.toObject();

                    QNDataItem item;
                    item.m_name = object.value("key").toString();
                    item.m_hash = object.value("hash").toString();
                    item.m_mimeType = object.value("mimeType").toString();
                    item.m_size = object.value("fsize").toInt();
                    item.m_putTime = object.value("putTime").toInt();
                    items << item;
                }
            }
        }
        reply->deleteLater();
        emit receiveFinshed( items );
    }
    else
    {
        emit receiveFinshed( QNDataItems() );
    }
}

void QNSimpleListData::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "error" << error;
}
