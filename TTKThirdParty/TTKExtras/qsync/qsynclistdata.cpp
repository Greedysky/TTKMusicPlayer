#include "qsynclistdata.h"
#include "qsyncdatainterface_p.h"

#include <QDateTime>
#include <QtXml/QDomDocument>

QSyncListData::QSyncListData(QNetworkAccessManager *networkManager, QObject *parent)
    : QSyncDataInterface(networkManager, parent)
{

}

void QSyncListData::request(const QString &bucket)
{
    TTK_D(QSyncDataInterface);
    const QString &method = "GET";
    const QString &url = TTK_SEPARATOR;
    const QString &resource = TTK_SEPARATOR + bucket + url;
    const QString &host = bucket + TTK_DOT + QSyncConfig::HOST;

    TTKStringMap headers;
    headers.insert("Host", host);
    headers.insert("Date", QSyncUtils::GMT());
    headers.insert("Content-Type", "charset=utf-8");

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(HTTP_PROTOCOL + host + url);

    for(auto it = headers.constBegin(); it != headers.constEnd(); ++it)
    {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void QSyncListData::receiveDataFromServer()
{
    QSyncDataItemList items;
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        QDomDocument docment;
        if(docment.setContent(reply->readAll()))
        {
            const QDomNodeList &nodes = docment.elementsByTagName("Contents");
            for(int i = 0; i < nodes.count(); ++i)
            {
                QSyncDataItem item;
                const QDomNodeList &childList = nodes.item(i).childNodes();
                for(int j = 0; j < childList.count(); ++j)
                {
                     const QDomNode &node = childList.item(j);
                     const QString &name = node.nodeName();
                     const QString &text = node.toElement().text();

                     if(name == "Key")
                     {
                         item.m_name = text;
                     }
                     else if(name == "LastModified")
                     {
                         QDateTime date = QDateTime::fromString(text, "yyyy-MM-ddThh:mm:ss.zzzZ");
                         date = date.addSecs(8 * 60 * 60);
                         item.m_putTime = date.toString(TTK_DATE_TIMEM_FORMAT);
                     }
                     else if(name == "ETag")
                     {
                         item.m_hash = text;
                         item.m_hash.remove(0, 1);
                         item.m_hash.chop(1);
                     }
                     else if(name == "Size")
                     {
                         item.m_size = text.toInt();
                     }
                     else if(name == "Type")
                     {
                         item.m_mimeType = text;
                     }
                     else if(name == "Owner")
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
