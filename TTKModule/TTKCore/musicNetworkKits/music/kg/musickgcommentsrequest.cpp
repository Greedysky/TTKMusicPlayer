#include "musickgcommentsrequest.h"
#include "musickgqueryrequest.h"

MusicKGSongCommentsRequest::MusicKGSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = COMMENT_PAGE_SIZE;
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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    TTKEventLoop loop;
    MusicKGQueryRequest query(this), *req = &query;
    connect(req, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    req->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    req->startToSearch(value);
    loop.exec();

    m_id.clear();
    if(!req->isEmpty())
    {
        MusicCommentsRequest::startToSearch(req->items().front().m_songId);
    }
}

void MusicKGSongCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicCommentsRequest::downLoadFinished();
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}



MusicKGPlaylistCommentsRequest::MusicKGPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = COMMENT_PAGE_SIZE;
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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGPlaylistCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicCommentsRequest::downLoadFinished();
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
