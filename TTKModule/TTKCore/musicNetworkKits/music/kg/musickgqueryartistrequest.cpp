#include "musickgqueryartistrequest.h"

MusicKGQueryArtistRequest::MusicKGQueryArtistRequest(QObject *parent)
    : MusicQueryArtistRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistRequest::startToSearch(const QString &artist)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), artist));

    deleteAll();
    m_queryText = artist;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ARTIST_URL, false).arg(artist).arg(0).arg(50));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryArtistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistRequest::downLoadFinished();
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
                bool artistFound = false;
                //
                value = value["data"].toMap();
                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["filename"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    if(musicInfo.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = musicInfo.m_songName.split(TTK_DEFAULT_STR);
                        musicInfo.m_singerName = MusicUtils::String::charactersReplaced(ll.front().trimmed());
                        musicInfo.m_songName = MusicUtils::String::charactersReplaced(ll.back().trimmed());
                    }

                    musicInfo.m_songId = value["hash"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    MusicResultsItem albumInfo;
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongAlbumInfo(&albumInfo, musicInfo.m_albumId);
                    musicInfo.m_albumName = albumInfo.m_nickName;
                    TTK_NETWORK_QUERY_CHECK();

                    readFromMusicSongLrcAndPicture(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!artistFound)
                    {
                        artistFound = true;
                        MusicResultsItem info;
                        TTK_NETWORK_QUERY_CHECK();
                        downLoadIntro(&info);
                        TTK_NETWORK_QUERY_CHECK();

                        info.m_id = m_queryText;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_coverUrl;
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

void MusicKGQueryArtistRequest::downLoadIntro(MusicResultsItem *item) const
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ARTIST_INFO_URL, false).arg(m_queryText));
    MusicKGInterface::makeRequestRawHeader(&request);

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
        if(value["errcode"].toInt() == 0)
        {
            value = value["data"].toMap();
            item->m_description = value["intro"].toString();
        }
    }
}
