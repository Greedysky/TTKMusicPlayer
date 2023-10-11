#include "musictxdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

#define ART_BACKGROUND_URL  "dGJmTlZOK1QvMDJENUxjMDk5UVhBWHVCb001eWtnQ1hKSnhsRWxLczNvRm9FV0kwbHhocTk4aml5SCs1Ym5mQU44SU05c1VZYVFzR2hLTEpGQ0hCNmM1ZUlZVnhnMm92QXNGMFN3PT0="
#define TX_SEARCH_URL       "RVNEYVc1Z1l4Wk9Ma2QxMEl3bnNnNEJaS0hiZVlZNnBQZXlQTUIzZGFjMksyVkNPMkxaeXBnPT0="
#define TX_SEARCH_DATA_URL  "bW8ybE03WEw1WmNmYzBuSTJWeTdPMFZNY0U2RDRYcjZla1djWmU2eU5ld2lZbytpaExzaG1MaDZ6U1FKTzVuWkZqZHJaL0gxN3ZiOTBIaFZwcmdpeU5NQllkQ1RaUzdVVmZ2UjFuWkF5SzlZSHYwajRKWEFXTUNBaGNab0V6U0UzOVdkMlpqM25CS212dEI2aU5KSUtLanJrcE5jU3JJUEEwSjhuVzFJV2xrM0FaWnFrUmladWxIUTlKUzF3RE5NR1RnUzJMNzRBTWF3Q0lLRmltZWc4QUh3b0dlZ1lxMTNXSXJLOGlWTlBEMmV4TkdlL2NzT0lqRW5UMHc9"

MusicTXImageConfigManager::MusicTXImageConfigManager(QObject *parent)
    : TTKXmlDocument(parent)
{

}

bool MusicTXImageConfigManager::readBuffer(QStringList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("url");
    for(int i = 0; i < nodes.count(); ++i)
    {
        items << nodes.item(i).toElement().text();
    }

    return true;
}


MusicTXDownloadBackgroundRequest::MusicTXDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicAbstractDownloadImageRequest(name, path, parent)
{

}

void MusicTXDownloadBackgroundRequest::startRequest()
{
    TTK_INFO_STREAM(QString("%1 startRequest").arg(className()));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(TX_SEARCH_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.post(request, TTK::Algorithm::mdII(TX_SEARCH_DATA_URL, false).arg(m_name).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicTXDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadDataFinished").arg(className()));

    MusicAbstractDownloadImageRequest::downLoadFinished();
    QString id;
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["music.search.SearchCgiService"].toMap();

            if(value.contains("code") && value["code"].toInt() == 0)
            {
                value = value["data"].toMap();
                value = value["body"].toMap();
                value = value["song"].toMap();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    id = value["id"].toString();
                    break;
                }
            }
        }
    }

    if(id.isEmpty())
    {
        deleteAll();
        return;
    }

    downLoadUrl(id);
}

void MusicTXDownloadBackgroundRequest::downLoadUrlFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadUrlFinished").arg(className()));

    MusicAbstractDownloadImageRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QStringList items;
        MusicTXImageConfigManager manager;
        if(manager.fromByteArray(m_reply->readAll()))
        {
            manager.readBuffer(items);
        }

        for(const QString &url : qAsConst(items))
        {
            if(m_counter < MAX_IMAGE_COUNT)
            {
                MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(m_counter++).arg(SKN_FILE), TTK::Download::Background, this);
                connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
                d->startRequest();
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString::number(m_counter));
    if(m_counter == 0)
    {
        deleteAll();
    }
}

void MusicTXDownloadBackgroundRequest::downLoadUrl(const QString &id)
{
    TTK_INFO_STREAM(QString("%1 downLoadUrl %2").arg(className(), id));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(id));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}
