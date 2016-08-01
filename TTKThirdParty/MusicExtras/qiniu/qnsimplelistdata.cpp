#include "qnsimplelistdata.h"
#include "qnmac.h"
#include "qniohelper.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonParseError>
#   include <QJsonDocument>
#   include <QJsonObject>
#   include <QJsonArray>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
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
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
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
            QScriptEngine engine;
            QScriptValue sc = engine.evaluate("value=" + QString(reply->readAll()));
            if(sc.property("items").isArray())
            {
                QScriptValueIterator it(sc.property("items"));
                while(it.hasNext())
                {
                    it.next();
                    QScriptValue value = it.value();
                    if(value.isNull() || value.property("key").toString().isEmpty())
                    {
                        continue;
                    }

                    QNDataItem item;
                    item.m_name = value.property("key").toString();
                    item.m_hash = value.property("hash").toString();
                    item.m_mimeType = value.property("mimeType").toString();
                    item.m_size = value.property("fsize").toInt32();
                    item.m_putTime = value.property("putTime").toInt32();
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
