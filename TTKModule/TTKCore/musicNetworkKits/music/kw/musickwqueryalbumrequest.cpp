#include "musickwqueryalbumrequest.h"

MusicKWQueryAlbumRequest::MusicKWQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryAlbumRequest::startToSearch(const QString &album)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), album));

    deleteAll();
    m_queryText = album;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ALBUM_URL, false).arg(album));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWQueryAlbumRequest::startToSingleSearch(const QString &artist)
{
    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(className(), artist));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_ALBUM_URL, false).arg(artist));
    MusicKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWQueryAlbumRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryAlbumRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(!value.isEmpty() && value.contains("musiclist"))
            {
                bool albumFound = false;
                //
                MusicResultsItem info;
                const QString &albumName = value["name"].toString();
                info.m_nickName = value["albumid"].toString();
                info.m_coverUrl = value["pic"].toString();
                if(!info.m_coverUrl.contains(HTTP_PREFIX) && !info.m_coverUrl.contains(TTK_NULL_STR))
                {
                    info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + info.m_coverUrl;
                }
                info.m_description = albumName + TTK_SPLITER +
                                     value["lang"].toString() + TTK_SPLITER +
                                     value["company"].toString() + TTK_SPLITER +
                                     value["pub"].toString();
                //
                const QVariantList &datas = value["musiclist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["artist"].toString());
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                    musicInfo.m_duration = TTK_DEFAULT_STR;

                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_artistId = value["artistid"].toString();
                    musicInfo.m_albumId = info.m_nickName;
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(albumName);

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongPicture(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    readFromMusicSongProperty(&musicInfo, value["formats"].toString(), m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!findUrlFileSize(&musicInfo.m_songProps)) return;
                    //
                    if(!albumFound)
                    {
                        albumFound = true;
                        info.m_id = musicInfo.m_albumId;
                        info.m_name = musicInfo.m_singerName;
                        Q_EMIT createAlbumInfoItem(info);
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

void MusicKWQueryAlbumRequest::downLoadSingleFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadSingleFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("albumlist"))
            {
                const QVariantList &datas = value["albumlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = value["albumid"].toString();
                    info.m_coverUrl = value["pic"].toString();
                    if(!info.m_coverUrl.contains(HTTP_PREFIX) && !info.m_coverUrl.contains(TTK_NULL_STR))
                    {
                        info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false) + info.m_coverUrl;
                    }
                    info.m_name = value["name"].toString();
                    info.m_updateTime = value["pub"].toString().replace(TTK_DEFAULT_STR, TTK_DOT);
                    Q_EMIT createAlbumInfoItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
