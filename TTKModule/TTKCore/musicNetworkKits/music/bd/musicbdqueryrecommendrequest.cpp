#include "musicbdqueryrecommendrequest.h"

MusicBDQueryRecommendRequest::MusicBDQueryRecommendRequest(QObject *parent)
    : MusicQueryRecommendRequest(parent)
{
    m_queryServer = QUERY_BD_INTERFACE;
}

void MusicBDQueryRecommendRequest::startToSearch()
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_RECOMMEND_URL, false));
    MusicBDInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicBDQueryRecommendRequest::downLoadFinished()
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
            const QVariantList &datas = data.toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                MusicObject::MusicSongInformation musicInfo;
                musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                musicInfo.m_duration = value["songLength"].toString();
                musicInfo.m_songId = value["id"].toString();

                const QVariantMap &albumObject = value["albumInfo"].toMap();
                musicInfo.m_albumId = albumObject["id"].toString();
                musicInfo.m_albumName = MusicUtils::String::charactersReplaced(albumObject["name"].toString());

                const QVariantList &artistsArray = value["artistInfo"].toList();
                for(const QVariant &artistValue : qAsConst(artistsArray))
                {
                    if(artistValue.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &artistObject = artistValue.toMap();
                    musicInfo.m_artistId = artistObject["id"].toString();
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(artistObject["name"].toString());
                    break; //just find first singer
                }

                musicInfo.m_year =  value["year"].toString();
                musicInfo.m_discNumber = value["disc"].toString();
                musicInfo.m_trackNumber = value["trackNum"].toString();

                TTK_NETWORK_QUERY_CHECK();
                readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                TTK_NETWORK_QUERY_CHECK();

                if(musicInfo.m_songProps.isEmpty())
                {
                    continue;
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

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
