#include "musicwycommentsrequest.h"
#include "musicwyquerysongrequest.h"

static constexpr const char *WY_COMMENT_SONG_URL = "VjJRaU8vR2xYZmIxM0RIMUNxU2NJRzlLSDVqZTY5ZVVXNlhUWndSWkc1TmlEajFKaGRwaS9TRThKRCtyL0Z4SWVSWmovSzhqMzFKV1R3QmxwUVBHSzA5ZGx0d2VmN0JMaVRFaVRnPT0=";
static constexpr const char *WY_COMMENT_PLAYLIST_URL = "aURTWlNhUlJISEc3VmExQ2FsUXZqS1lXOVdGSzBOc3dmaHNMK3M0U2tScnNIdHc3Y2NJSGJBVVhhUTc1eHIrNEJrdmgzUlpCOU96UmNsZEEwbnIvQkZQU0tFZU9oY0JKazNlRWZRPT0=";
static constexpr const char *WY_COMMENT_DATA_URL = "c0RITC8rR0FPY1VXdVpzT2Rsb0hJYVBITUM5cDR4d0YxTERqRzJMUUpPWlhUemt3Z2l0M0dMNTQ2MkJ0aVdCTUZaYW5pdkRDWTdsbTE5aEM=";

MusicWYSongCommentsRequest::MusicWYSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicWYSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_SONG_URL, false).arg(m_id),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_id).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    TTKEventLoop loop;
    MusicWYQuerySongRequest query(this), *req = &query;
    connect(req, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
    req->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    req->startToSearch(value);
    loop.exec();

    m_id.clear();
    if(!req->isEmpty())
    {
        MusicCommentsRequest::startToSearch(req->items().first().m_songId);
    }
}

void MusicWYSongCommentsRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicCommentsRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    MusicResultDataItem item;
                    const QVariantMap &user = value["user"].toMap();
                    item.m_nickName = user["nickname"].toString();
                    item.m_coverUrl = user["avatarUrl"].toString();
                    item.m_count = value["likedCount"].toString();
                    item.m_time = value["time"].toString();
                    item.m_description = value["content"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicWYPlaylistCommentsRequest::MusicWYPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicWYPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_PLAYLIST_URL, false).arg(m_id),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_id).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYPlaylistCommentsRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicCommentsRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    MusicResultDataItem item;
                    const QVariantMap &user = value["user"].toMap();
                    item.m_nickName = user["nickname"].toString();
                    item.m_coverUrl = user["avatarUrl"].toString();
                    item.m_count = value["likedCount"].toString();
                    item.m_time = value["time"].toString();
                    item.m_description = value["content"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
