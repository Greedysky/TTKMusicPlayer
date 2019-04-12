#include "musicdownloadquerybdartistthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDArtistThread::MusicDownLoadQueryBDArtistThread(QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicDownLoadQueryBDArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_ARTIST_URL, false).arg(artist).arg(0).arg(50);
    m_interrupt = true;
    m_searchText = artist;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDArtistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
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
            if(value["error_code"].toInt() == 22000 && value.contains("songlist"))
            {
                bool artistFlag = false;
                //
                const QVariantList &datas = value["songlist"].toList();
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
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["file_duration"].toInt()*1000);

                    musicInfo.m_songId = value["song_id"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();
                    musicInfo.m_artistId = value["ting_uid"].toString();
                    musicInfo.m_lrcUrl = value["lrclink"].toString();
                    musicInfo.m_smallPicUrl = value["pic_small"].toString().replace("_90", "_500");
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["album_title"].toString());

                    musicInfo.m_year = value["publishtime"].toString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = value["album_no"].toString();

                    if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicResultsItem info;
                        if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        getDownLoadIntro(&info);
                        if(m_interrupt || m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        info.m_id = m_searchText;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        emit createArtistInfoItem(info);
                    }
                    //
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
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDArtistThread::getDownLoadIntro(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_ARTIST_INFO_URL, false).arg(m_searchText);

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
        item->m_tags = value["country"].toString();
        item->m_updateTime = value["birth"].toString();
        item->m_nickName = value["nickname"].toString();
        item->m_description = value["intro"].toString();
    }
}
