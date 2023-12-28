#include "musickgcommentsrequest.h"
#include "musickgqueryrequest.h"

MusicKGSongCommentsRequest::MusicKGSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKGSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_COMMENT_SONG_URL, false).arg(m_rawData["sid"].toString()).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    TTKSemaphoreLoop loop;
    MusicKGQueryRequest query(this), *d = &query;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    d->startToSearch(value);
    loop.exec();

    m_rawData["sid"].clear();
    if(!d->isEmpty())
    {
        m_rawData["sid"] = d->songInfoList().front().m_songId;
        startToPage(0);
    }
}

void MusicKGSongCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["err_code"].toInt() == 0)
            {
                m_totalSize = value["count"].toLongLong();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_playCount = value["like"].toMap()["count"].toString();
                    result.m_updateTime = TTKDateTime::format(value["addtime"].toString(), TTK_YEAR_TIMEZ_FORMAT);
                    result.m_description = value["content"].toString();
                    result.m_nickName = value["user_name"].toString();
                    result.m_coverUrl = value["user_pic"].toString();
                    Q_EMIT createCommentItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}



MusicKGPlaylistCommentsRequest::MusicKGPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKGPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_COMMENT_PLAYLIST_URL, false).arg(m_rawData["sid"].toString()).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGPlaylistCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    m_rawData["sid"] = value;
    startToPage(0);
}

void MusicKGPlaylistCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["err_code"].toInt() == 0)
            {
                m_totalSize = value["count"].toLongLong();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_playCount = value["like"].toMap()["count"].toString();
                    result.m_updateTime = TTKDateTime::format(value["addtime"].toString(), TTK_YEAR_TIMEZ_FORMAT);
                    result.m_description = value["content"].toString();
                    result.m_nickName = value["user_name"].toString();
                    result.m_coverUrl = value["user_pic"].toString();
                    Q_EMIT createCommentItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
