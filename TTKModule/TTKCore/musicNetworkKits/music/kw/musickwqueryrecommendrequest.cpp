#include "musickwqueryrecommendrequest.h"

MusicKWQueryRecommendRequest::MusicKWQueryRecommendRequest(QObject *parent)
    : MusicQueryRecommendRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryRecommendRequest::startToSearch()
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_RECOMMEND_URL, false).arg(24005).arg(50));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKWQueryRecommendRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryRecommendRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(!value.isEmpty() && value.contains("musiclist"))
            {
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
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_artistId = value["artistid"].toString();
                    musicInfo.m_albumId = value["albumid"].toString();
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(value["album"].toString());

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
