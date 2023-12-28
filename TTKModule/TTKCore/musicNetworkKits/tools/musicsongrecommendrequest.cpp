#include "musicsongrecommendrequest.h"

static constexpr const char *LQ_BASE_URL = "VzJWczlXM2hMeCtTZzhLRFJvZWxUUTVmZUVBLzlMWmo=";
static constexpr const char *LQ_RECOMMEND_URL = "NDhiOGZ6dUJWNTBvN3R5OHNOQmkyQVVwOXdWbDNBOG14MmVXWVJxWlVXRkxuNUxxdzdYTEpUYVZRNVE9";

namespace MusicLQInterface
{
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    static void parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    static void parseFromSongProperty(TTK::MusicSongInformation *info, const QString &key, int length, int bitrate);

}

void MusicLQInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    info->m_lrcUrl = TTK::Algorithm::mdII(LQ_BASE_URL, false) + key["lrcUrl"].toString();
    info->m_coverUrl = key["picUrl"].toString();
    const int length = key["length"].toInt();

    parseFromSongProperty(info, key["lqUrl"].toString(), length, TTK_BN_128);
    parseFromSongProperty(info, key["hqUrl"].toString(), length, TTK_BN_192);
    parseFromSongProperty(info, key["sqUrl"].toString(), length, TTK_BN_320);
    parseFromSongProperty(info, key["apeUrl"].toString(), length, TTK_BN_750);
    parseFromSongProperty(info, key["flacUrl"].toString(), length, TTK_BN_1000);

    if(info->m_songProps.isEmpty())
    {
        parseFromSongProperty(info, key["copyUrl"].toString(), length, TTK_BN_128);
    }
}

void MusicLQInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &key, int length, int bitrate)
{
    if(key.isEmpty())
    {
        return;
    }

    TTK::MusicSongProperty prop;
    prop.m_url = TTK::Algorithm::mdII(LQ_BASE_URL, false) + key;
    prop.m_size = TTK::Number::sizeByteToLabel(length * 1000 * bitrate / 8);
    prop.m_bitrate = bitrate;

    switch(bitrate)
    {
        case TTK_BN_128: prop.m_format = MP3_FILE_SUFFIX; break;
        case TTK_BN_192: prop.m_format = MP3_FILE_SUFFIX; break;
        case TTK_BN_320: prop.m_format = MP3_FILE_SUFFIX; break;
        case TTK_BN_750: prop.m_format = APE_FILE_SUFFIX; break;
        case TTK_BN_1000: prop.m_format = FLAC_FILE_SUFFIX; break;
        default: prop.m_format = MP3_FILE_SUFFIX; break;
    }
    info->m_songProps.append(prop);
}


MusicSongRecommendRequest::MusicSongRecommendRequest(QObject *parent)
    : MusicAbstractQueryRequest(parent)
{

}

void MusicSongRecommendRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(LQ_RECOMMEND_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicSongRecommendRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

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
                MusicLQInterface::parseFromSongProperty(&info, value);
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
