#include "qsynclistdata.h"
#include "qsyncdatainterface_p.h"

#include <QDateTime>
#include <QtXml/QDomDocument>

QSyncListData::QSyncListData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{

}

void QSyncListData::listDataOperator(const QString &bucket)
{
    TTK_D(QSyncDataInterface);
    const QString &method = "GET";
    const QString &url = TTK_SEPARATOR;
    const QString &resource = TTK_SEPARATOR + bucket + url;
    const QString &host = bucket + TTK_DOT + QSyncConfig::HOST;

    TTKStringMap headers;
    headers.insert("Date", QSyncUtils::GMT());
    headers.insert("Host", host);
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl("http://" + host + url);

    TTKStringMapIter it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void QSyncListData::receiveDataFromServer()
{
    QSyncDataItemList items;
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        QDomDocument docment;
        if(docment.setContent(reply->readAll()))
        {
            const QDomNodeList &nodeList = docment.elementsByTagName("Contents");
            for(int i = 0; i < nodeList.count(); ++i)
            {
                QSyncDataItem item;
                const QDomNodeList &childList = nodeList.at(i).childNodes();
                for(int j = 0; j < childList.count(); ++j)
                {
                     const QDomNode &node = childList.at(j);
                     if(node.nodeName() == "Key")
                     {
                         item.m_name = node.toElement().text();
                     }
                     else if(node.nodeName() == "LastModified")
                     {
                         QDateTime date = QDateTime::fromString(node.toElement().text(), "yyyy-MM-ddThh:mm:ss.zzzZ");
                         date = date.addSecs(8*60*60);
                         item.m_putTime = date.toString("yyyy-MM-dd hh:mm");
                     }
                     else if(node.nodeName() == "ETag")
                     {
                         item.m_hash = node.toElement().text();
                         item.m_hash.remove(0, 1);
                         item.m_hash.chop(1);
                     }
                     else if(node.nodeName() == "Size")
                     {
                         item.m_size = node.toElement().text().toInt();
                     }
                     else if(node.nodeName() == "Type")
                     {
                         item.m_mimeType = node.toElement().text();
                     }
                     else if(node.nodeName() == "Owner")
                     {
                         items << item;
                         item.clear();
                     }
                }
            }
        }
        reply->deleteLater();
    }
    Q_EMIT receiveFinshed(items);
}
