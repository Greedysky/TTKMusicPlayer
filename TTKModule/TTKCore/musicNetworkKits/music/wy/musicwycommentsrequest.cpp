#include "musicwycommentsrequest.h"
#include "musicwyqueryrequest.h"

MusicWYSongCommentsRequest::MusicWYSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = COMMENT_PAGE_SIZE;
}

void MusicWYSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_SONG_URL, false).arg(m_id),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_id).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    TTKSemaphoreLoop loop;
    MusicWYQueryRequest query(this), *d = &query;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    d->startToSearch(value);
    loop.exec();

    m_id.clear();
    if(!d->isEmpty())
    {
        MusicCommentsRequest::startToSearch(d->items().front().m_songId);
    }
}

void MusicWYSongCommentsRequest::downLoadFinished()
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
            if(value["code"].toInt() == 200)
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    const QVariantMap &user = value["user"].toMap();
                    result.m_nickName = user["nickname"].toString();
                    result.m_coverUrl = user["avatarUrl"].toString();
                    result.m_count = value["likedCount"].toString();
                    result.m_updateTime = value["time"].toString();
                    result.m_description = value["content"].toString();
                    Q_EMIT createCommentItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}



MusicWYPlaylistCommentsRequest::MusicWYPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = COMMENT_PAGE_SIZE;
}

void MusicWYPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_PLAYLIST_URL, false).arg(m_id),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_id).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYPlaylistCommentsRequest::downLoadFinished()
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
            if(value["code"].toInt() == 200)
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    const QVariantMap &user = value["user"].toMap();
                    result.m_nickName = user["nickname"].toString();
                    result.m_coverUrl = user["avatarUrl"].toString();
                    result.m_count = value["likedCount"].toString();
                    result.m_updateTime = value["time"].toString();
                    result.m_description = value["content"].toString();
                    Q_EMIT createCommentItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
