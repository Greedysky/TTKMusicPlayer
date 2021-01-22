#include "musicqqqueryplaylistrequest.h"
#include "musicsemaphoreloop.h"

#define REFER_URL   "cmw5eWcwaXhCSDYvU1RoR3U5ZFV2NVppeDlyVkwxbFZtYnJZK3g5SzViaHFXc3FYQ09RblRYVHhORGVFVmFnN0RRUm5jZz09"

MusicQQQueryPlaylistRequest::MusicQQQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryPlaylistRequest::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist.isEmpty() ? "10000000" : playlist;
        startToPage(0);
    }
}

void MusicQQQueryPlaylistRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_URL, false).arg(m_searchText).arg(m_pageSize*offset).arg(m_pageSize*(offset + 1) - 1));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQQueryPlaylistRequest::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(playlist));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQQueryPlaylistRequest::getPlaylistInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 getPlaylistInfo %2").arg(getClassName()).arg(item.m_id));

    setNetworkAbort(false);

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(item.m_id));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(reply->readAll(), &ok);
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
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
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
                    getMoreDetails(&item);
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

void MusicQQQueryPlaylistRequest::getDetailsFinished()
{
    TTK_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(reply->readAll(), &ok);
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
                            musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(name["name"].toString());
                            musicInfo.m_artistId = name["mid"].toString();
                        }
                        musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songname"].toString());
                        musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                        m_rawData["songID"] = value["songid"].toString();
                        musicInfo.m_songId = value["songmid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();
                        musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumname"].toString());

                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                        musicInfo.m_smallPicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(musicInfo.m_albumId);

                        musicInfo.m_year = QString();
                        musicInfo.m_discNumber = value["cdIdx"].toString();
                        musicInfo.m_trackNumber = value["belongCD"].toString();

                        TTK_NETWORK_QUERY_CHECK();
                        readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                        TTK_NETWORK_QUERY_CHECK();

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }
                        //
                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_time = musicInfo.m_timeLength;
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

void MusicQQQueryPlaylistRequest::getMoreDetails(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 getMoreDetails %2").arg(getClassName()).arg(item->m_id));

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_PLAYLIST_INFO_URL, false).arg(item->m_id));
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(reply->readAll(), &ok);
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
