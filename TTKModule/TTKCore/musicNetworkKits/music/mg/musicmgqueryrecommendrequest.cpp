#include "musicmgqueryrecommendrequest.h"

MusicMGQueryRecommendRequest::MusicMGQueryRecommendRequest(QObject *parent)
    : MusicQueryRecommendRequest(parent)
{
    m_queryServer = QUERY_MG_INTERFACE;
}

void MusicMGQueryRecommendRequest::startToSearch(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(id));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(QString(""));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGQueryRecommendRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    setNetworkAbort(false);

    TTK_LOGGER_INFO(m_reply->readAll());
//    if(m_reply && m_reply->error() == QNetworkReply::NoError)
//    {
//        QJson::Parser parser;
//        bool ok;
//        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
//        if(ok)
//        {
//            QVariantMap value = data.toMap();
//            if(value.contains("result"))
//            {
//                value = value["result"].toMap();
//                const QVariantList &datas = value["results"].toList();
//                for(const QVariant &var : qAsConst(datas))
//                {
//                    if(var.isNull())
//                    {
//                        continue;
//                    }

//                    value = var.toMap();
//                    value = value["songData"].toMap();
//                    MusicObject::MusicSongInformation musicInfo;
//                    const QVariantList &singerName = value["singerName"].toList();
//                    if(singerName.size() == 0)
//                    {
//                        continue;
//                    }

//                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(singerName.first().toString());
//                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songName"].toString());
//                    musicInfo.m_timeLength = STRING_NULL;

//                    musicInfo.m_songId = value["songId"].toString();
//                    musicInfo.m_artistId = value["singerId"].toString();
//                    musicInfo.m_albumId = value["albumId"].toString();
//                    musicInfo.m_smallPicUrl = value["cover"].toString();

//                    musicInfo.m_year = QString();
//                    musicInfo.m_discNumber = "1";
//                    musicInfo.m_trackNumber = "0";

//                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(value["copyrightId"].toString());
//                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumName"].toString());

//                    TTK_NETWORK_QUERY_CHECK();
//                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
//                    TTK_NETWORK_QUERY_CHECK();

//                    if(musicInfo.m_songAttrs.isEmpty())
//                    {
//                        continue;
//                    }
//                    //
//                    if(!findUrlFileSize(&musicInfo.m_songAttrs)) return;
//                    //
//                    setTimeLength(&musicInfo);
//                    //
//                    MusicSearchedItem item;
//                    item.m_songName = musicInfo.m_songName;
//                    item.m_singerName = musicInfo.m_singerName;
//                    item.m_albumName = musicInfo.m_albumName;
//                    item.m_time = musicInfo.m_timeLength;
//                    item.m_type = mapQueryServerString();
//                    Q_EMIT createSearchedItem(item);
//                    m_musicSongInfos << musicInfo;
//                }
//            }
//        }
//    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
