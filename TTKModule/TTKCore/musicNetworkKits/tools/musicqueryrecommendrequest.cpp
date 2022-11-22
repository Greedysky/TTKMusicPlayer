#include "musicqueryrecommendrequest.h"

#define LQ_BASE_URL         "VzJWczlXM2hMeCtTZzhLRFJvZWxUUTVmZUVBLzlMWmo="
#define LQ_RECOMMEND_URL    "NDhiOGZ6dUJWNTBvN3R5OHNOQmkyQVVwOXdWbDNBOG14MmVXWVJxWlVXRkxuNUxxdzdYTEpUYVZRNVE9"

MusicQueryRecommendRequest::MusicQueryRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicQueryRecommendRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    Q_UNUSED(value);
    startToSearch();
}

void MusicQueryRecommendRequest::startToSearch()
{
    TTK_INFO_STREAM(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(LQ_RECOMMEND_URL, false));

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicQueryRecommendRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
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

void MusicQueryRecommendRequest::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QString &key, int length, int bitrate) const
{
    if(key.isEmpty())
    {
        return;
    }

    MusicObject::MusicSongProperty prop;
    prop.m_url = MusicUtils::Algorithm::mdII(LQ_BASE_URL, false) + key;
    prop.m_size = MusicUtils::Number::sizeByte2Label(length * 1000 * bitrate / 8);
    switch(bitrate)
    {
        case MB_128: prop.m_format = MP3_FILE_PREFIX; break;
        case MB_192: prop.m_format = MP3_FILE_PREFIX; break;
        case MB_320: prop.m_format = MP3_FILE_PREFIX; break;
        case MB_750: prop.m_format = APE_FILE_PREFIX; break;
        case MB_1000: prop.m_format = FLAC_FILE_PREFIX; break;
        default: prop.m_format = MP3_FILE_PREFIX; break;
    }
    prop.m_bitrate = bitrate;
    info->m_songProps.append(prop);
}

void MusicQueryRecommendRequest::parseFromSongProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key, MusicObject::QueryQuality quality, bool all) const
{
    info->m_lrcUrl = MusicUtils::Algorithm::mdII(LQ_BASE_URL, false) + key["lrcUrl"].toString();
    info->m_coverUrl = key["picUrl"].toString();
    const int length = key["length"].toInt();

    if(all)
    {
        parseFromSongProperty(info, key["lqUrl"].toString(), length, MB_128);
        parseFromSongProperty(info, key["hqUrl"].toString(), length, MB_192);
        parseFromSongProperty(info, key["sqUrl"].toString(), length, MB_320);
        parseFromSongProperty(info, key["apeUrl"].toString(), length, MB_750);
        parseFromSongProperty(info, key["flacUrl"].toString(), length, MB_1000);
    }
    else
    {
        if(quality == MusicObject::QueryQuality::Standard)
        {
            parseFromSongProperty(info, key["lqUrl"].toString(), length, MB_128);
        }
        else if(quality == MusicObject::QueryQuality::High)
        {
            parseFromSongProperty(info, key["hqUrl"].toString(), length, MB_192);
        }
        else if(quality == MusicObject::QueryQuality::Super)
        {
            parseFromSongProperty(info, key["sqUrl"].toString(), length, MB_320);
        }
        else if(quality == MusicObject::QueryQuality::Lossless)
        {
            parseFromSongProperty(info, key["apeUrl"].toString(), length, MB_750);
            parseFromSongProperty(info, key["flacUrl"].toString(), length, MB_1000);
        }
    }

    if(info->m_songProps.isEmpty())
    {
        parseFromSongProperty(info, key["copyUrl"].toString(), length, MB_128);
    }
}
