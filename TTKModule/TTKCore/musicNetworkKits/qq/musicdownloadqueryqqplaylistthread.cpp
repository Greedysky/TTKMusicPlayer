#include "musicdownloadqueryqqplaylistthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#define REFER_URL   "cmw5eWcwaXhCSDYvU1RoR3U5ZFV2NVppeDlyVkwxbFZtYnJZK3g5SzViaHFXc3FYQ09RblRYVHhORGVFVmFnN0RRUm5jZz09"

MusicDownLoadQueryQQPlaylistThread::MusicDownLoadQueryQQPlaylistThread(QObject *parent)
    : MusicDownLoadQueryPlaylistThread(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicDownLoadQueryQQPlaylistThread::startToSearch(QueryType type, const QString &playlist)
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

void MusicDownLoadQueryQQPlaylistThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_URL, false).arg(m_searchText).arg(m_pageSize*offset).arg(m_pageSize*(offset + 1) - 1);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryQQPlaylistThread::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_ATTR_URL, false).arg(playlist);
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryQQPlaylistThread::getPlaylistInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getPlaylistInfo %2").arg(getClassName()).arg(item.m_id));
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_ATTR_URL, false).arg(item.m_id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    const QByteArray &bytes = reply->readAll();

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("cdlist"))
        {
            const QVariantList &datas = value["cdlist"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                item.m_coverUrl = value["logo"].toString();
                item.m_name = value["dissname"].toString();
                item.m_playCount = QString::number(value["listennum"].toULongLong());
                item.m_description = value["desc"].toString();
                item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["ctime"].toULongLong()*1000).toString("yyyy-MM-dd");
                item.m_nickName = value["nickname"].toString();

                const QVariantList &tags = value["tags"].toList();
                QString tagsString;
                foreach(const QVariant &tag, tags)
                {
                    if(tag.isNull())
                    {
                        continue;
                    }

                    tagsString.append(tag.toMap()["name"].toString() + "|");
                }
                item.m_tags = tagsString;
            }
        }
    }
}

void MusicDownLoadQueryQQPlaylistThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_pageTotal = value["sum"].toLongLong();
                const QVariantList &datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem item;
                    item.m_coverUrl = value["imgurl"].toString();
                    item.m_id = value["dissid"].toString();
                    item.m_name = value["dissname"].toString();
                    item.m_playCount = QString::number(value["listennum"].toULongLong());
                    item.m_description = value["introduction"].toString();
                    item.m_updateTime = value["commit_time"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    getMoreDetails(&item);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    value = value["creator"].toMap();
                    item.m_nickName = value["name"].toString();

                    emit createPlaylistItem(item);
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryQQPlaylistThread::getDetailsFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    M_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(reply && m_manager && reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("cdlist"))
            {
                const QVariantList &datas = value["cdlist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    const QVariantList &songLists = value["songlist"].toList();
                    foreach(const QVariant &var, songLists)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        MusicObject::MusicSongInformation musicInfo;
                        foreach(const QVariant &var, value["singer"].toList())
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
                        musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt()*1000);

                        m_rawData["songID"] = value["songid"].toString();
                        musicInfo.m_songId = value["songmid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();
                        musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumname"].toString());

                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                        musicInfo.m_smallPicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false)
                                                  .arg(musicInfo.m_albumId.right(2).left(1))
                                                  .arg(musicInfo.m_albumId.right(1)).arg(musicInfo.m_albumId);

                        musicInfo.m_year = QString();
                        musicInfo.m_discNumber = value["cdIdx"].toString();
                        musicInfo.m_trackNumber = value["belongCD"].toString();

                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_time = musicInfo.m_timeLength;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItem(item);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    M_LOGGER_INFO(QString("%1 getDetailsFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryQQPlaylistThread::getMoreDetails(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getMoreDetails %2").arg(getClassName()).arg(item->m_id));
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_ATTR_URL, false).arg(item->m_id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    const QByteArray &bytes = reply->readAll();

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 0 && value.contains("cdlist"))
        {
            const QVariantList &datas = value["cdlist"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                const QVariantList &tags = value["tags"].toList();
                QString tagsString;
                foreach(const QVariant &tag, tags)
                {
                    if(tag.isNull())
                    {
                        continue;
                    }

                    tagsString.append(tag.toMap()["name"].toString() + "|");
                }
                item->m_tags = tagsString;
                item->m_description = value["desc"].toString();
            }
        }
    }
}
