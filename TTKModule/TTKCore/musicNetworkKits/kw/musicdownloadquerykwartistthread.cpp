#include "musicdownloadquerykwartistthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWArtistThread::MusicDownLoadQueryKWArtistThread(QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicDownLoadQueryKWArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_URL, false).arg(artist).arg(0).arg(50);
    m_searchText = artist;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWArtistThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes.replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                bool artistFlag = false;
                //
                const QVariantList &datas = value["abslist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["ARTIST"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["SONGNAME"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["DURATION"].toInt()*1000);

                    musicInfo.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                    musicInfo.m_artistId = value["ARTISTID"].toString();
                    musicInfo.m_albumId = value["ALBUMID"].toString();
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["ALBUM"].toString());

                    musicInfo.m_year = value["RELEASEDATE"].toString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicSongPic(&musicInfo);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    ///music normal songs urls
                    readFromMusicSongAttribute(&musicInfo, value["FORMATS"].toString(), m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
                    //
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicResultsItem info;
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        getDownLoadIntro(&info);
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        info.m_id = musicInfo.m_artistId;
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

void MusicDownLoadQueryKWArtistThread::getDownLoadIntro(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(m_searchText);

    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
    const QVariant &data = parser.parse(reply->readAll().replace("'", "\""), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        item->m_tags = value["country"].toString();
        item->m_updateTime = value["birthday"].toString();
        item->m_nickName = value["aartist"].toString();
        item->m_description = value["info"].toString();

        item->m_description.replace("&nbsp;", " ");
        item->m_description.replace("&lt;br&gt;", "\r\n");
    }
}
