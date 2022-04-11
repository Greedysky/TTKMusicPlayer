#include "musicqqqueryplaylistrequest.h"

#define REFER_URL   "cmw5eWcwaXhCSDYvU1RoR3U5ZFV2NVppeDlyVkwxbFZtYnJZK3g5SzViaHFXc3FYQ09RblRYVHhORGVFVmFnN0RRUm5jZz09"

MusicQQQueryPlaylistRequest::MusicQQQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryPlaylistRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == MusicQuery)
    {
        startToSearch(value);
    }
    else
    {
        m_queryValue = value.isEmpty() ? "10000000" : value;
        startToPage(0);
    }
}

void MusicQQQueryPlaylistRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_URL, false).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize * (offset + 1) - 1));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(value));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryPlaylistRequest::queryPlaylistInfo(MusicResultsItem &item)
{
    TTK_LOGGER_INFO(QString("%1 queryPlaylistInfo %2").arg(className(), item.m_id));

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(item.m_id));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("cdlist"))
        {
            const QVariantList &datas = value["cdlist"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                item.m_coverUrl = value["logo"].toString();
                item.m_name = value["dissname"].toString();
                item.m_playCount = QString::number(value["listennum"].toULongLong());
                item.m_description = value["desc"].toString();
                item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["ctime"].toULongLong() * 1000).toString(MUSIC_YEAR_FORMAT);
                item.m_nickName = value["nickname"].toString();

                item.m_tags.clear();
                const QVariantList &tags = value["tags"].toList();
                for(const QVariant &tag : qAsConst(tags))
                {
                    if(tag.isNull())
                    {
                        continue;
                    }

                    item.m_tags.append(tag.toMap()["name"].toString() + "|");
                }
            }
        }
    }
}

void MusicQQQueryPlaylistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryPlaylistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["sum"].toLongLong();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_coverUrl = value["imgurl"].toString();
                    item.m_id = value["dissid"].toString();
                    item.m_name = value["dissname"].toString();
                    item.m_playCount = QString::number(value["listennum"].toULongLong());
                    item.m_description = value["introduction"].toString();
                    item.m_updateTime = value["commit_time"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    moreDetails(&item);
                    TTK_NETWORK_QUERY_CHECK();

                    value = value["creator"].toMap();
                    item.m_nickName = value["name"].toString();

                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicQQQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_LOGGER_INFO(QString("%1 downloadDetailsFinished").arg(className()));

    MusicQueryPlaylistRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("cdlist"))
            {
                const QVariantList &datas = value["cdlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    const QVariantList &songs = value["songlist"].toList();
                    for(const QVariant &var : qAsConst(songs))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        TTK_NETWORK_QUERY_CHECK();

                        MusicObject::MusicSongInformation musicInfo;
                        for(const QVariant &var : value["singer"].toList())
                        {
                            if(var.isNull())
                            {
                                continue;
                            }

                            const QVariantMap &name = var.toMap();
                            musicInfo.m_singerName = MusicUtils::String::charactersReplaced(name["name"].toString());
                            musicInfo.m_artistId = name["mid"].toString();
                            break; //just find first singer
                        }
                        musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                        musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                        m_rawData["sid"] = value["songid"].toString();
                        musicInfo.m_songId = value["songmid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();
                        musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["albumname"].toString());

                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                        musicInfo.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(musicInfo.m_albumId);

                        musicInfo.m_year = QString();
                        musicInfo.m_discNumber = value["cdIdx"].toString();
                        musicInfo.m_trackNumber = value["belongCD"].toString();

                        TTK_NETWORK_QUERY_CHECK();
                        readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                        TTK_NETWORK_QUERY_CHECK();

                        if(musicInfo.m_songProps.isEmpty())
                        {
                            continue;
                        }
                        //
                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_duration = musicInfo.m_duration;
                        item.m_type = mapQueryServerString();
                        Q_EMIT createSearchedItem(item);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
}

void MusicQQQueryPlaylistRequest::moreDetails(MusicResultsItem *item) const
{
    TTK_LOGGER_INFO(QString("%1 moreDetails %2").arg(className(), item->m_id));

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(item->m_id));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    MusicQQInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("cdlist"))
        {
            const QVariantList &datas = value["cdlist"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                item->m_description = value["desc"].toString();
                item->m_tags.clear();
                const QVariantList &tags = value["tags"].toList();
                for(const QVariant &tag : qAsConst(tags))
                {
                    if(tag.isNull())
                    {
                        continue;
                    }

                    item->m_tags.append(tag.toMap()["name"].toString() + "|");
                }
            }
        }
    }
}
