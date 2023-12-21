#include "musicwycommentsrequest.h"
#include "musicwyqueryrequest.h"

MusicWYSongCommentsRequest::MusicWYSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicWYSongCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_SONG_URL, false).arg(m_rawData["sid"].toInt()),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_rawData["sid"].toInt()).arg(m_pageSize).arg(m_pageSize * offset));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicWYSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    TTKSemaphoreLoop loop;
    MusicWYQueryRequest *d = new MusicWYQueryRequest(this);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->startToSearch(MusicAbstractQueryRequest::QueryType::Music, value);
    loop.exec();

    m_rawData["sid"] = 0;
    if(!d->isEmpty())
    {
        m_rawData["sid"] = d->songInfoList().front().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWYSongCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

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
                m_totalSize = value["total"].toLongLong();

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
                    result.m_playCount = value["likedCount"].toString();
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
    m_pageSize = 20;
}

void MusicWYPlaylistCommentsRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    m_rawData["sid"] = value;
    startToPage(0);
}

void MusicWYPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_COMMENT_PLAYLIST_URL, false).arg(m_rawData["sid"].toLongLong()),
                      TTK::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_rawData["sid"].toLongLong()).arg(m_pageSize).arg(m_pageSize * offset));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicWYPlaylistCommentsRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

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
                m_totalSize = value["total"].toLongLong();

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
                    result.m_playCount = value["likedCount"].toString();
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
