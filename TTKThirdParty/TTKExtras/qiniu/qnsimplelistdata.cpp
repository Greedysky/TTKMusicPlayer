#include "qnsimplelistdata.h"
#include "qnmac.h"
#include "qniohelper.h"
#///QJson import
#include "qjson/parser.h"

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
    const QNMac mac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
    QNetworkRequest request = QNIOHelper::listRequest(bucket, &mac);
    QNetworkReply *reply = d->m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
}

void QNSimpleListData::receiveDataFromServer()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply)
    {
        QNDataItems items;
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
        {
            QJson::Parser parser;
            bool ok;
            const QVariant &data = parser.parse(reply->readAll(), &ok);
            if(ok)
            {
                QVariantMap value = data.toMap();
                const QVariantList &array = value["items"].toList();
                foreach(const QVariant &var, array)
                {
                    value = var.toMap();
                    QNDataItem item;
                    item.m_name = value["key"].toString();
                    item.m_hash = value["hash"].toString();
                    item.m_mimeType = value["mimeType"].toString();
                    item.m_size = value["fsize"].toInt();
                    item.m_putTime = value["putTime"].toLongLong()/10000;
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
