#include "musickwcommentsrequest.h"
#include "musickwqueryrequest.h"

MusicKWSongCommentsRequest::MusicKWSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicKWSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_COMMENT_SONG_URL, false).arg(m_id).arg(offset + 1).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    TTKEventLoop loop;
    MusicKWQueryRequest query(this), *req = &query;
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

void MusicKWSongCommentsRequest::downloadFinished()
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
            if(value["result"].toString() == "ok")
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["rows"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_count = value["like_num"].toString();
                    item.m_time = TTKDateTime::format(value["time"].toString(), TTK_DATE_TIMES_FORMAT);
                    item.m_description = value["msg"].toString();
                    item.m_nickName = QUrl::fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    item.m_coverUrl = value["u_pic"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKWPlaylistCommentsRequest::MusicKWPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_20;
}

void MusicKWPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_COMMENT_PLAYLIST_URL, false).arg(m_id).arg(offset + 1).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWPlaylistCommentsRequest::downloadFinished()
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
            if(value["result"].toString() == "ok")
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["rows"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_count = value["like_num"].toString();
                    item.m_time = TTKDateTime::format(value["time"].toString(), TTK_DATE_TIMES_FORMAT);
                    item.m_description = value["msg"].toString();
                    item.m_nickName = QUrl::fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    item.m_coverUrl = value["u_pic"].toString();
                    Q_EMIT createCommentItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
