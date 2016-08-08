#include "qnsimplelistdata.h"
#include "qnmac.h"
#include "qniohelper.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonParseError>
#   include <QJsonDocument>
#   include <QJsonObject>
#   include <QJsonArray>
#else
#   ///QJson import
#   include "qjson/parser.h"
#endif
#include <QDebug>

class QNSimpleListDataPrivate : public TTKPrivate<QNSimpleListData>
{
public:
    QNSimpleListDataPrivate();

    QNetworkAccessManager *m_networkManager;

};

QNSimpleListDataPrivate::QNSimpleListDataPrivate()
{

}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
QNSimpleListData::QNSimpleListData(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(QNSimpleListData);
    d->m_networkManager = networkManager;
}

void QNSimpleListData::listDataToServer(const QString &bucket)
{
    TTK_D(QNSimpleListData);
    QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNetworkRequest request = QNIOHelper::listRequest(bucket, &mac);
    QNetworkReply *reply = d->m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                   SLOT(handleError(QNetworkReply::NetworkError)));
}

void QNSimpleListData::receiveDataFromServer()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        QNDataItems items;
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
        {
#ifdef MUSIC_GREATER_NEW
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
                    item.m_size = object.value("fsize").toVariant().toInt();
                    item.m_putTime = object.value("putTime").toVariant().toInt();
                    items << item;
                }
            }
#else
            QJson::Parser parser;
            bool ok;
            QVariant data = parser.parse(reply->readAll(), &ok);
            if(ok)
            {
                QVariantMap value = data.toMap();
                QVariantList array = value["items"].toList();
                foreach(QVariant var, array)
                {
                    value = var.toMap();
                    QNDataItem item;
                    item.m_name = value["key"].toString();
                    item.m_hash = value["hash"].toString();
                    item.m_mimeType = value["mimeType"].toString();
                    item.m_size = value["fsize"].toInt();
                    item.m_putTime = value["putTime"].toInt();
                    items << item;
                }
            }
#endif
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
