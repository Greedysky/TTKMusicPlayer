#include "musicabstractqueryrequest.h"

MusicAbstractQueryRequest::MusicAbstractQueryRequest(QObject *parent)
    : MusicPageQueryRequest(parent)
{
    m_queryServer = "Invalid";
    m_queryQuality = MusicObject::StandardQuality;
    m_queryAllRecords = false;
    m_queryLite = false;
}

void MusicAbstractQueryRequest::startToSingleSearch(const QString &value)
{
    Q_UNUSED(value);
}

QString MusicAbstractQueryRequest::mapQueryServerString() const
{
    const QString &v = tr("Current used server from %1");
    if(m_queryServer.contains(QUERY_BD_INTERFACE))
        return v.arg(tr("BD"));
    else if(m_queryServer.contains(QUERY_KG_INTERFACE))
        return v.arg(tr("KG"));
    else if(m_queryServer.contains(QUERY_KW_INTERFACE))
        return v.arg(tr("KW"));
    else if(m_queryServer.contains(QUERY_QQ_INTERFACE))
        return v.arg(tr("QQ"));
    else if(m_queryServer.contains(QUERY_WY_INTERFACE))
        return v.arg(tr("WY"));
    else
        return QString();
}

qint64 MusicAbstractQueryRequest::fileSizeByUrl(const QString &url)
{
    qint64 size = -1;

    QNetworkRequest request;
    request.setUrl(url);
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.head(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    QObject::connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#else
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#endif
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return size;
    }

    size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    const QVariant &redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirection.isValid())
    {
        size = fileSizeByUrl(redirection.toString());
    }

    reply->deleteLater();
    return size;
}

void MusicAbstractQueryRequest::downLoadFinished()
{
    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    MusicPageQueryRequest::downLoadFinished();
}

bool MusicAbstractQueryRequest::findUrlFileSize(MusicObject::MusicSongProperty *prop) const
{
    TTK_NETWORK_QUERY_CHECK(false);
    if(prop->m_size.isEmpty() || prop->m_size == TTK_DEFAULT_STR)
    {
        prop->m_size = MusicUtils::Number::sizeByte2Label(fileSizeByUrl(prop->m_url));
    }
    TTK_NETWORK_QUERY_CHECK(false);
    return true;
}

bool MusicAbstractQueryRequest::findUrlFileSize(MusicObject::MusicSongPropertyList *props) const
{
    for(int i = 0; i < props->count(); ++i)
    {
        if(!findUrlFileSize(&(*props)[i]))
        {
            return false;
        }
    }
    return true;
}
