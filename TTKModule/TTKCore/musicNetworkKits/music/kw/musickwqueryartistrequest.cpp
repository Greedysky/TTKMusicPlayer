#include "musickwqueryartistrequest.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"

MusicKWQueryArtistRequest::MusicKWQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistRequest::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));

    deleteAll();
    m_queryText = artist;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_URL, false).arg(artist).arg(0).arg(50));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWQueryArtistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    MusicQueryArtistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                bool artistFound = false;
                //
                const QVariantList &datas = value["abslist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["ARTIST"].toString());
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["SONGNAME"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["DURATION"].toInt() * 1000);

                    musicInfo.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                    musicInfo.m_artistId = value["ARTISTID"].toString();
                    musicInfo.m_albumId = value["ALBUMID"].toString();
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["ALBUM"].toString());

                    musicInfo.m_year = value["RELEASEDATE"].toString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongPicture(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    readFromMusicSongAttribute(&musicInfo, value["FORMATS"].toString(), m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
                    //
                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultsItem info;
                        TTK_NETWORK_QUERY_CHECK();
                        getDownLoadIntro(&info);
                        TTK_NETWORK_QUERY_CHECK();
                        info.m_id = musicInfo.m_artistId;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        Q_EMIT createArtistInfoItem(info);
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

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKWQueryArtistRequest::getDownLoadIntro(MusicResultsItem *item)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(m_queryText));
    MusicKWInterface::makeRequestRawHeader(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    QObject::connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#else
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#endif
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(reply->readAll().replace("'", "\""), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        item->m_tags = value["country"].toString();
        item->m_updateTime = value["birthday"].toString();
        item->m_nickName = value["aartist"].toString();
        item->m_description = MusicUtils::String::convertHtmlToPlain(value["info"].toString());
    }
}
