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
    QtNetworkErrorConnect(m_reply, this, replyError);
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

                    MusicObject::MusicSongInformation info;
                    info.m_singerName = MusicUtils::String::charactersReplaced(value["artist"].toString());
                    info.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                    info.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    info.m_songId = value["id"].toString();
                    info.m_artistId = value["artistid"].toString();
                    info.m_albumId = value["albumid"].toString();
                    info.m_albumName = MusicUtils::String::charactersReplaced(value["album"].toString());

                    info.m_year = QString();
                    info.m_discNumber = "1";
                    info.m_trackNumber = "0";

                    info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_COVER_URL, false).arg(info.m_songId);
                    info.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);
                    readFromMusicSongProperty(&info, value["formats"].toString(), m_queryQuality, m_queryAllRecords);

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
                    }

                    if(!findUrlFileSize(&info.m_songProps))
                    {
                        return;
                    }

                    MusicSearchedItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
                    item.m_albumName = info.m_albumName;
                    item.m_duration = info.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
