#include "musicdownloadquerybdplaylistthread.h"
#include "musicsemaphoreloop.h"
#include "musicotherdefine.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDPlaylistThread::MusicDownLoadQueryBDPlaylistThread(QObject *parent)
    : MusicDownLoadQueryPlaylistThread(parent)
{
    m_pageSize = 20;
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicDownLoadQueryBDPlaylistThread::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist.isEmpty() ? "%E5%8D%8E%E8%AF%AD" : playlist;
        startToPage(0);
    }
}

void MusicDownLoadQueryBDPlaylistThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_PLAYLIST_URL, false).arg(m_searchText).arg(offset).arg(m_pageSize);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDPlaylistThread::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_PLAYLIST_ATTR_URL, false).arg(playlist);
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDPlaylistThread::getPlaylistInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getPlaylistInfo %2").arg(getClassName()).arg(item.m_id));
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_PLAYLIST_ATTR_URL, false).arg(item.m_id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
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
        const QVariantMap &value = data.toMap();
        if(value["error_code"].toInt() == 22000)
        {
            item.m_coverUrl = value["pic_300"].toString();
            item.m_name = value["title"].toString();
            item.m_playCount = value["listenum"].toString();
            item.m_description = value["desc"].toString();
            item.m_updateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
            item.m_nickName = MUSIC_AUTHOR_NAME;
            item.m_tags = value["tag"].toString().replace(",", "|");
        }
    }
}

void MusicDownLoadQueryBDPlaylistThread::downLoadFinished()
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
            if(value["error_code"].toInt() == 22000)
            {
                m_pageTotal = value["total"].toLongLong();
                const QVariantList &datas = value["content"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem item;
                    item.m_coverUrl = value["pic_300"].toString();
                    item.m_id = value["listid"].toString();
                    item.m_name = value["title"].toString();
                    item.m_playCount = value["listenum"].toString();
                    item.m_description = value["desc"].toString();
                    item.m_updateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
                    item.m_nickName = MUSIC_AUTHOR_NAME;
                    item.m_tags = value["tag"].toString().replace(",", "|");

                    emit createPlaylistItem(item);
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDPlaylistThread::getDetailsFinished()
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
            if(value["error_code"].toInt() == 22000 && value.contains("content"))
            {
                const QVariantList &datas = value["content"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["author"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());
                    musicInfo.m_timeLength = "-";

                    musicInfo.m_songId = value["song_id"].toString();
                    musicInfo.m_artistId = value["ting_uid"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["album_title"].toString());

                    musicInfo.m_lrcUrl = value["lrclink"].toString();
                    musicInfo.m_smallPicUrl = value["pic_big"].toString().replace("_90", "_500");

                    musicInfo.m_year = value["publishtime"].toString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = value["album_no"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    musicInfo.m_timeLength = findTimeStringByAttrs(musicInfo.m_songAttrs);
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

    emit downLoadDataChanged(QString());
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
