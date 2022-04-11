#include "musickwcommentsrequest.h"
#include "musickwqueryrequest.h"

MusicKWSongCommentsRequest::MusicKWSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKWSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    MusicSemaphoreLoop loop;
    MusicKWQueryRequest *d = new MusicKWQueryRequest(this);
    d->setQueryAllRecords(false);
    d->setQueryLite(true);
    d->startToSearch(MusicAbstractQueryRequest::MusicQuery, value);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["sid"].clear();
    if(!d->isEmpty())
    {
        m_rawData["sid"] = d->musicSongInfoList().front().m_songId;
        startToPage(0);
    }
}

void MusicKWSongCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_COMMENT_SONG_URL, false).arg(m_rawData["sid"].toString()).arg(offset + 1).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWSongCommentsRequest::downLoadFinished()
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

                    MusicResultsItem item;
                    item.m_playCount = value["like_num"].toString();
                    item.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(), MUSIC_YEAR_STIME_FORMAT).toMSecsSinceEpoch());
                    item.m_description = value["msg"].toString();
                    item.m_nickName = QUrl::fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    item.m_coverUrl = value["u_pic"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}



MusicKWPlaylistCommentsRequest::MusicKWPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKWPlaylistCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    m_rawData["sid"] = value;
    startToPage(0);
}

void MusicKWPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_COMMENT_PLAYLIST_URL, false).arg(m_rawData["sid"].toString()).arg(offset + 1).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWPlaylistCommentsRequest::downLoadFinished()
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

                    MusicResultsItem item;
                    item.m_playCount = value["like_num"].toString();
                    item.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(), MUSIC_YEAR_STIME_FORMAT).toMSecsSinceEpoch());
                    item.m_description = value["msg"].toString();
                    item.m_nickName = QUrl().fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    item.m_coverUrl = value["u_pic"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
