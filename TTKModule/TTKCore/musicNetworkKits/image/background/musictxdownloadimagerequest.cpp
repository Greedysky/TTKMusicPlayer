#include "musictxdownloadimagerequest.h"
#include "musicdownloaddatarequest.h"

static constexpr const char *ART_BACKGROUND_URL = "dGJmTlZOK1QvMDJENUxjMDk5UVhBWHVCb001eWtnQ1hKSnhsRWxLczNvRm9FV0kwbHhocTk4aml5SCs1Ym5mQU44SU05c1VZYVFzR2hLTEpGQ0hCNmM1ZUlZVnhnMm92QXNGMFN3PT0=";
static constexpr const char *TX_SEARCH_URL = "RVNEYVc1Z1l4Wk9Ma2QxMEl3bnNnNEJaS0hiZVlZNnBQZXlQTUIzZGFjMksyVkNPMkxaeXBnPT0=";
static constexpr const char *TX_SEARCH_DATA_URL = "bW8ybE03WEw1WmNmYzBuSTJWeTdPMFZNY0U2RDRYcjZla1djWmU2eU5ld2lZbytpaExzaG1MaDZ6U1FKTzVuWkZqZHJaL0gxN3ZiOTBIaFZwcmdpeU5NQllkQ1RaUzdVVmZ2UjFuWkF5SzlZSHYwajRKWEFXTUNBaGNab0V6U0UzOVdkMlpqM25CS212dEI2aU5KSUtLanJrcE5jU3JJUEEwSjhuVzFJV2xrM0FaWnFrUmladWxIUTlKUzF3RE5NR1RnUzJMNzRBTWF3Q0lLRmltZWc4QUh3b0dlZ1lxMTNXSXJLOGlWTlBEMmV4TkdlL2NzT0lqRW5UMHc9";

MusicTXImageConfigManager::MusicTXImageConfigManager()
    : TTKAbstractXml()
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

void MusicTXDownloadBackgroundRequest::startToRequest()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(TX_SEARCH_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.post(request, TTK::Algorithm::mdII(TX_SEARCH_DATA_URL, false).arg(m_name).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicTXDownloadBackgroundRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

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

            if(value["code"].toInt() == 0)
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
        TTK_INFO_STREAM(className() << __FUNCTION__);
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    downLoadUrl(id);
}

void MusicTXDownloadBackgroundRequest::downLoadUrlFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

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
            if(m_counter < m_remainCount && !url.isEmpty())
            {
                MusicDownloadDataRequest *d = new MusicDownloadDataRequest(url, QString("%1%2-%3%4").arg(BACKGROUND_DIR_FULL, m_path).arg(foundCount()).arg(SKN_FILE), TTK::Download::Background, this);
                connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadDataFinished()));
                d->startToRequest();
            }
        }
    }

    TTK_INFO_STREAM(className() << "download image size" << m_counter);
    Q_EMIT downLoadDataChanged(QString::number(m_counter));
    //
    if(m_counter == 0)
    {
        deleteAll();
    }
}

void MusicTXDownloadBackgroundRequest::downLoadUrl(const QString &id)
{
    TTK_INFO_STREAM(className() << __FUNCTION__ << id);

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ART_BACKGROUND_URL, false).arg(id));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}
