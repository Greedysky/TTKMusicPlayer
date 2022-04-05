#include "musicqqqueryalbumrequest.h"

MusicQQQueryAlbumRequest::MusicQQQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryAlbumRequest::startToSearch(const QString &album)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), album));

    deleteAll();
    m_queryText = album;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ALBUM_URL, false).arg(album));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryAlbumRequest::startToSingleSearch(const QString &artist)
{
    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(className(), artist));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_ARTIST_ALBUM_URL, false).arg(artist));
    MusicQQInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQQueryAlbumRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryAlbumRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                bool albumFound = false;
                //
                MusicResultsItem info;
                value = value["data"].toMap();
                info.m_description = TTK_SPLITER +
                                     value["lan"].toString() + TTK_SPLITER +
                                     value["company_new"].toMap()["name"].toString() + TTK_SPLITER +
                                     value["aDate"].toString();
                //
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
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
                        musicInfo.m_artistId = MusicUtils::String::charactersReplaced(name["mid"].toString());
                        break; //just find first singer
                    }
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                    m_rawData["sid"] = value["songid"].toString();
                    musicInfo.m_songId = value["songmid"].toString();
                    musicInfo.m_albumId = value["albummid"].toString();
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    musicInfo.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(musicInfo.m_albumId);
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["albumname"].toString());

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
                    if(!albumFound)
                    {
                        albumFound = true;
                        info.m_name = musicInfo.m_singerName;
                        info.m_id = musicInfo.m_albumId;
                        info.m_description = musicInfo.m_albumName + info.m_description;
                        info.m_coverUrl = musicInfo.m_coverUrl;
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

void MusicQQQueryAlbumRequest::downLoadSingleFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadSingleFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = value["albumMID"].toString();
                    info.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(info.m_id);
                    info.m_name = value["albumName"].toString();
                    info.m_updateTime = value["pubTime"].toString().replace(TTK_DEFAULT_STR, TTK_DOT);
                    Q_EMIT createAlbumInfoItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
