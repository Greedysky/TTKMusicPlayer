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
    QtNetworkErrorConnect(m_reply, this, replyError);
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

                MusicObject::MusicSongInformation info;
                info.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                info.m_duration = value["songLength"].toString();
                info.m_songId = value["id"].toString();

                const QVariantMap &albumObject = value["albumInfo"].toMap();
                info.m_albumId = albumObject["id"].toString();
                info.m_albumName = MusicUtils::String::charactersReplaced(albumObject["name"].toString());

                const QVariantList &artistsArray = value["artistInfo"].toList();
                for(const QVariant &artistValue : qAsConst(artistsArray))
                {
                    if(artistValue.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &artistObject = artistValue.toMap();
                    info.m_artistId = artistObject["id"].toString();
                    info.m_singerName = MusicUtils::String::charactersReplaced(artistObject["name"].toString());
                    break; //just find first singer
                }

                info.m_year =  value["year"].toString();
                info.m_discNumber = value["disc"].toString();
                info.m_trackNumber = value["trackNum"].toString();

                TTK_NETWORK_QUERY_CHECK();
                parseFromSongProperty(&info, value, m_queryQuality, m_queryAllRecords);
                TTK_NETWORK_QUERY_CHECK();

                if(info.m_songProps.isEmpty())
                {
                    continue;
                }

                MusicResultInfoItem item;
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

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
