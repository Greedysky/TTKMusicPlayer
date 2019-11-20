#include "osslistdata.h"
#include "ossdatainterface_p.h"

#include <QDateTime>
#include <QtXml/QDomDocument>

OSSListData::OSSListData(QNetworkAccessManager *networkManager, QObject *parent)
    : OSSDataInterface(networkManager, parent)
{

}

void OSSListData::listDataOperator(const QString &bucket)
{
    TTK_D(OSSDataInterface);
    const QString &method = "GET";
    const QString &url = "/";
    const QString &resource = "/" + bucket + "/";
    const QString &host = bucket + "." + OSSConf::OSS_HOST;

    MStringMap headers;
    headers.insert("Date", OSSUtils::getGMT());
    headers.insert("Host", host);

    d->insertAuthorization(method, headers, resource);

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + host + url));

    MStringMapterator it(headers);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = d->m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(receiveDataFromServer()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError(QNetworkReply::NetworkError)));
}

void OSSListData::receiveDataFromServer()
{
    OSSDataItems items;
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200)
    {
        QDomDocument docment;
        if(docment.setContent(reply->readAll()))
        {
            const QDomNodeList &nodeList = docment.elementsByTagName("Contents");
            for(int i=0; i<nodeList.count(); ++i)
            {
                OSSDataItem item;
                const QDomNodeList &childList = nodeList.at(i).childNodes();
                for(int j=0; j<childList.count(); ++j)
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
    emit receiveFinshed(items);
}
