#include "musicwyqueryplaylistrequest.h"
#include "musicsemaphoreloop.h"

MusicWYQueryPlaylistRequest::MusicWYQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryPlaylistRequest::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist.isEmpty() ? "all" : playlist;
        startToPage(0);
    }
}

void MusicWYQueryPlaylistRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_URL, false),
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_DATA_URL, false).arg(m_searchText).arg(m_pageSize).arg(m_pageSize*offset));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYQueryPlaylistRequest::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));

    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_INFO_URL, false),
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_INFO_DATA_URL, false).arg(playlist));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYQueryPlaylistRequest::getPlaylistInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 getPlaylistInfo %2").arg(getClassName()).arg(item.m_id));

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_INFO_URL, false),
                      MusicUtils::Algorithm::mdII(WY_PLAYLIST_INFO_DATA_URL, false).arg(item.m_id));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->post(request, parameter);
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
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("playlist"))
        {
            value = value["playlist"].toMap();
            item.m_coverUrl = value["coverImgUrl"].toString();
            item.m_name = value["name"].toString();
            item.m_playCount = QString::number(value["playCount"].toULongLong());
            item.m_description = value["description"].toString();
            item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["updateTime"].toULongLong()).toString(MUSIC_YEAR_FORMAT);

            const QVariantList &tags = value["tags"].toList();
            item.m_tags.clear();
            for(const QVariant &var : qAsConst(tags))
            {
                if(var.isNull())
                {
                    continue;
                }
                item.m_tags.append(var.toString() + "|");
            }

            value = value["creator"].toMap();
            item.m_nickName = value["nickname"].toString();
        }
    }
}

void MusicWYQueryPlaylistRequest::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    Q_EMIT clearAllItems();
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
            if(value["code"].toInt() == 200 && value.contains("playlists"))
            {
                m_pageTotal = value["total"].toLongLong();
                const QVariantList &datas = value["playlists"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem item;
                    item.m_coverUrl = value["coverImgUrl"].toString();
                    item.m_id = QString::number(value["id"].toULongLong());
                    item.m_name = value["name"].toString();
                    item.m_playCount = QString::number(value["playCount"].toULongLong());
                    item.m_description = value["description"].toString();
                    item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["updateTime"].toULongLong()).toString(MUSIC_YEAR_FORMAT);

                    const QVariantList &tags = value["tags"].toList();
                    item.m_tags.clear();
                    for(const QVariant &var : qAsConst(tags))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }
                        item.m_tags.append(var.toString() + "|");
                    }

                    value = value["creator"].toMap();
                    item.m_nickName = value["nickname"].toString();

                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicWYQueryPlaylistRequest::getDetailsFinished()
{
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());

    TTK_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));
    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(reply && reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("playlist"))
            {
                value = value["playlist"].toMap();
                const QVariantList &datas = value["tracks"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["name"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["dt"].toInt());
                    musicInfo.m_songId = QString::number(value["id"].toInt());
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(musicInfo.m_songId);

                    const QVariantMap &albumObject = value["al"].toMap();
                    musicInfo.m_smallPicUrl = albumObject["picUrl"].toString();
                    musicInfo.m_albumId = QString::number(albumObject["id"].toInt());
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(albumObject["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        const QVariantMap &artistMap = artistValue.toMap();
                        musicInfo.m_artistId = QString::number(artistMap["id"].toULongLong());
                        musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(artistMap["name"].toString());
                    }

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = value["cd"].toString();
                    musicInfo.m_trackNumber = value["no"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return;
                    readFromMusicSongAttributeNew(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return;

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
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
}
