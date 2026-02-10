#include "musickgcommentsrequest.h"
#include "musickgquerysongrequest.h"

static constexpr const char *KG_COMMENT_SONG_URL = "WklSbmRxMm50MkdJOS91Y0Q1b09ET0IwMlZtNTVKRitGS3BCdkxyUkJXeHkwVGNMeVFZa29weTVEa2VFSFEzUGQvQi9DanNLNnVmcDhEdzU0bUYrMUtpOWNNaXhOTUlUWFF3RG9JSlFhbEpIMndaWS90c2xBb1Q4SWY2dG91NjYxNCtoZDJ3czI5ZU54U2ZlcTdjcGpVNTd1dFR6MGh0Tk5mNGVLT2VXRHFHb1RNZmVaR0NhU1R2VTFhZ25yTVFI";
static constexpr const char *KG_COMMENT_PLAYLIST_URL = "TEZ4NXdPYnM4MkNJSTZMNUVjU2h5cGFvY0FzWmI2Lzc1RGM5UUJTRTdVcVZEQndyMGRQZ0x0cXJlb1E0dXVqRUZSWXRoSUR4V1lRUVRLQjVuNkdyUDdFZ2x1ZitLZml2eThxU2R2TEJHYUM5a1hDak5GSS9weStvb2JYRXhKL3hkMGVMdmVVbm5DOFEwL3BTMjd6N1VvY1pXYzRlcWdpNFJKVmZqa1hEOTRWUitrVEE3UTBWcEMwMllsMXJZSjV4S1FQSE9wWTlsdmtXQ1pYaA==";

MusicKGSongCommentsRequest::MusicKGSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicKGSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_COMMENT_SONG_URL, false).arg(m_id).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    TTKEventLoop loop;
    MusicKGQuerySongRequest query(this), *req = &query;
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

void MusicKGSongCommentsRequest::downloadFinished()
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
            if(value["err_code"].toInt() == 0)
            {
                m_totalSize = value["count"].toInt();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_count = value["like"].toMap()["count"].toString();
                    item.m_time = TTKDateTime::format(value["addtime"].toString(), TTK_DATE_TIMES_FORMAT);
                    item.m_description = value["content"].toString();
                    item.m_nickName = value["user_name"].toString();
                    item.m_coverUrl = value["user_pic"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGPlaylistCommentsRequest::MusicKGPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicKGPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_COMMENT_PLAYLIST_URL, false).arg(m_id).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGPlaylistCommentsRequest::downloadFinished()
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
            if(value["err_code"].toInt() == 0)
            {
                m_totalSize = value["count"].toInt();

                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_count = value["like"].toMap()["count"].toString();
                    item.m_time = TTKDateTime::format(value["addtime"].toString(), TTK_DATE_TIMES_FORMAT);
                    item.m_description = value["content"].toString();
                    item.m_nickName = value["user_name"].toString();
                    item.m_coverUrl = value["user_pic"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
