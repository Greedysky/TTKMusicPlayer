#include "musicsongrecommendrequest.h"

static constexpr const char *LQ_BASE_URL = "VzJWczlXM2hMeCtTZzhLRFJvZWxUUTVmZUVBLzlMWmo=";
static constexpr const char *LQ_RECOMMEND_URL = "NDhiOGZ6dUJWNTBvN3R5OHNOQmkyQVVwOXdWbDNBOG14MmVXWVJxWlVXRkxuNUxxdzdYTEpUYVZRNVE9";

MusicSongRecommendRequest::MusicSongRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicSongRecommendRequest::startToSearch(QueryType type, const QString &value)
{
    Q_UNUSED(type);
    Q_UNUSED(value);
    startToSearch();
}

void MusicSongRecommendRequest::startToSearch()
{
    TTK_INFO_STREAM(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(LQ_RECOMMEND_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicSongRecommendRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicAbstractQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
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

                TTK::MusicSongInformation info;
                info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                info.m_duration = value["songLength"].toString();
                info.m_songId = value["id"].toString();

                const QVariantMap &albumObject = value["albumInfo"].toMap();
                info.m_albumId = albumObject["id"].toString();
                info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                const QVariantList &artistsArray = value["artistInfo"].toList();
                for(const QVariant &artistValue : qAsConst(artistsArray))
                {
                    if(artistValue.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &artistObject = artistValue.toMap();
                    info.m_artistId = artistObject["id"].toString();
                    info.m_singerName = TTK::String::charactersReplace(artistObject["name"].toString());
                    break; //just find first singer
                }

                info.m_year =  value["year"].toString();
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicSongRecommendRequest::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &key, int length, int bitrate) const
{
    if(key.isEmpty())
    {
        return;
    }

    TTK::MusicSongProperty prop;
    prop.m_url = TTK::Algorithm::mdII(LQ_BASE_URL, false) + key;
    prop.m_size = TTK::Number::sizeByteToLabel(length * 1000 * bitrate / 8);
    switch(bitrate)
    {
        case MB_128: prop.m_format = MP3_FILE_SUFFIX; break;
        case MB_192: prop.m_format = MP3_FILE_SUFFIX; break;
        case MB_320: prop.m_format = MP3_FILE_SUFFIX; break;
        case MB_750: prop.m_format = APE_FILE_SUFFIX; break;
        case MB_1000: prop.m_format = FLAC_FILE_SUFFIX; break;
        default: prop.m_format = MP3_FILE_SUFFIX; break;
    }
    prop.m_bitrate = bitrate;
    info->m_songProps.append(prop);
}

void MusicSongRecommendRequest::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key, TTK::QueryQuality quality, bool all) const
{
    info->m_lrcUrl = TTK::Algorithm::mdII(LQ_BASE_URL, false) + key["lrcUrl"].toString();
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
        if(quality == TTK::QueryQuality::Standard)
        {
            parseFromSongProperty(info, key["lqUrl"].toString(), length, MB_128);
        }
        else if(quality == TTK::QueryQuality::High)
        {
            parseFromSongProperty(info, key["hqUrl"].toString(), length, MB_192);
        }
        else if(quality == TTK::QueryQuality::Super)
        {
            parseFromSongProperty(info, key["sqUrl"].toString(), length, MB_320);
        }
        else if(quality == TTK::QueryQuality::Lossless)
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
