#include "musicwycommentsrequest.h"
#include "musicwyqueryrequest.h"

MusicWYSongCommentsRequest::MusicWYSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicWYSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    MusicSemaphoreLoop loop;
    MusicWYQueryRequest *d = new MusicWYQueryRequest(this);
    d->setQueryAllRecords(false);
    d->setQueryLite(true);
    d->startToSearch(MusicAbstractQueryRequest::MusicQuery, value);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["sid"] = 0;
    if(!d->isEmpty())
    {
        m_rawData["sid"] = d->musicSongInfoList().front().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWYSongCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_COMMENT_SONG_URL, false).arg(m_rawData["sid"].toInt()),
                      MusicUtils::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_rawData["sid"].toInt()).arg(m_pageSize).arg(m_pageSize * offset));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicWYSongCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
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

                    MusicResultsItem item;
                    const QVariantMap &user = value["user"].toMap();
                    item.m_nickName = user["nickname"].toString();
                    item.m_coverUrl = user["avatarUrl"].toString();
                    item.m_playCount = QString::number(value["likedCount"].toLongLong());
                    item.m_updateTime = QString::number(value["time"].toLongLong());
                    item.m_description = value["content"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}



MusicWYPlaylistCommentsRequest::MusicWYPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicWYPlaylistCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    m_rawData["sid"] = value;
    startToPage(0);
}

void MusicWYPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_COMMENT_PLAYLIST_URL, false).arg(m_rawData["sid"].toLongLong()),
                      MusicUtils::Algorithm::mdII(WY_COMMENT_DATA_URL, false).arg(m_rawData["sid"].toLongLong()).arg(m_pageSize).arg(m_pageSize * offset));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicWYPlaylistCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
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

                    MusicResultsItem item;
                    const QVariantMap &user = value["user"].toMap();
                    item.m_nickName = user["nickname"].toString();
                    item.m_coverUrl = user["avatarUrl"].toString();
                    item.m_playCount = QString::number(value["likedCount"].toLongLong());
                    item.m_updateTime = QString::number(value["time"].toLongLong());
                    item.m_description = value["content"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
