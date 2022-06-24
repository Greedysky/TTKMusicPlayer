#include "musicqqqueryrecommendrequest.h"

MusicQQQueryRecommendRequest::MusicQQQueryRecommendRequest(QObject *parent)
    : MusicQueryRecommendRequest(parent)
{
    m_queryServer = QUERY_QQ_INTERFACE;
}

void MusicQQQueryRecommendRequest::startToSearch()
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_RECOMMEND_URL, false));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicQQQueryRecommendRequest::downLoadFinished()
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
            if(value["code"].toInt() == 0 && value.contains("new_song"))
            {
                value = value["new_song"].toMap();
                value = value["data"].toMap();
                const QVariantList &datas = value["song_list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation info;
                    for(const QVariant &var : value["singer"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &name = var.toMap();
                        info.m_singerName = MusicUtils::String::charactersReplaced(name["name"].toString());
                        info.m_artistId = name["mid"].toString();
                        break; //just find first singer
                    }
                    info.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                    info.m_duration = MusicTime::msecTime2LabelJustified(value["interval"].toInt() * 1000);

                    m_rawData["sid"] = value["id"].toString();
                    info.m_songId = value["mid"].toString();

                    const QVariantMap &albumObject = value["album"].toMap();
                    info.m_albumId = albumObject["mid"].toString();
                    info.m_albumName = MusicUtils::String::charactersReplaced(albumObject["name"].toString());
                    info.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(info.m_songId);
                    info.m_coverUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false).arg(info.m_albumId);

                    info.m_year = value["time_public"].toString();
                    info.m_discNumber = value["index_cd"].toString();
                    info.m_trackNumber = value["index_album"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongPropertyNew(&info, value["file"].toMap());
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
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
