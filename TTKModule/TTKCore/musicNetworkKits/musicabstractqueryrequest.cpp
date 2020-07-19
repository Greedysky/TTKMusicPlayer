#include "musicabstractqueryrequest.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"

MusicAbstractQueryRequest::MusicAbstractQueryRequest(QObject *parent)
    : MusicPagingRequest(parent)
{
    m_queryAllRecords = false;
    m_querySimplify = false;
    m_searchQuality = tr("SD");
    m_queryServer = "Invalid";
}

MusicAbstractQueryRequest::~MusicAbstractQueryRequest()
{
    deleteAll();
}

void MusicAbstractQueryRequest::startToSingleSearch(const QString &text)
{
    Q_UNUSED(text);
}

QString MusicAbstractQueryRequest::mapQueryServerString() const
{
    const QString &v = tr("Current Used Server Is %1");
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
    else if(m_queryServer.contains(QUERY_XM_INTERFACE))
        return v.arg(tr("XM"));
    else
        return QString();
}

QString MusicAbstractQueryRequest::findTimeStringByAttrs(const MusicObject::MusicSongAttributes &attrs)
{
    foreach(const MusicObject::MusicSongAttribute &attr, attrs)
    {
        if(!attr.m_duration.isEmpty())
        {
            return attr.m_duration;
        }
    }

    return QString("-");
}

bool MusicAbstractQueryRequest::findUrlFileSize(MusicObject::MusicSongAttribute *attr)
{
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return false;
    if(attr->m_size.isEmpty() || attr->m_size == "-")
    {
        attr->m_size = MusicUtils::Number::size2Label(getUrlFileSize(attr->m_url));
    }
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return false;

    return true;
}

bool MusicAbstractQueryRequest::findUrlFileSize(MusicObject::MusicSongAttributes *attrs)
{
    for(int i=0; i<attrs->count(); ++i)
    {
        if(!findUrlFileSize(&(*attrs)[i]))
        {
            return false;
        }
    }

    return true;
}

qint64 MusicAbstractQueryRequest::getUrlFileSize(const QString &url)
{
    qint64 size = -1;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(url);
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.head(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return size;
    }

    size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    const QVariant &redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(!redirection.isNull())
    {
        size = getUrlFileSize(redirection.toString());
    }

    reply->deleteLater();

    return size;
}
