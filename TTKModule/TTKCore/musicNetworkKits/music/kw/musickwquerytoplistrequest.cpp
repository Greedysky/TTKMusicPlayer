#include "musickwquerytoplistrequest.h"

MusicKWQueryToplistRequest::MusicKWQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();
    m_queryValue = value.isEmpty() ? "16" : value;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_TOPLIST_URL, false).arg(m_queryValue));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryToplistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");
    MusicPageQueryRequest::downLoadFinished();

    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlFileSize(&info->m_songProps, info->m_duration);
    MusicQueryToplistRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryToplistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicQueryToplistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("musiclist"))
            {
                queryToplistInfo(value);

                const QVariantList &datas = value["musiclist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_singerName = TTK::String::charactersReplace(value["artist"].toString());
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    info.m_songId = value["id"].toString();
                    info.m_artistId = value["artistid"].toString();
                    info.m_albumId = value["albumid"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album"].toString());

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    info.m_coverUrl = value["web_albumpic_short"].toString();
                    MusicKWInterface::makeCoverPixmapUrl(info.m_coverUrl, info.m_songId);
                    info.m_lrcUrl = TTK::Algorithm::mdII(KW_SONG_LRC_URL, false).arg(info.m_songId);

                    TTK_NETWORK_QUERY_CHECK();
                    MusicKWInterface::parseFromSongProperty(&info, value["formats"].toString());
                    TTK_NETWORK_QUERY_CHECK();

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
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryToplistRequest::queryToplistInfo(const QVariantMap &input)
{
    Q_UNUSED(input);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_TOPLIST_INFO_URL, false));
    MusicKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                const QVariantList &ranks = value["list"].toList();
                for(const QVariant &rank : qAsConst(ranks))
                {
                    if(rank.isNull())
                    {
                        continue;
                    }

                    value = rank.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_queryValue != value["sourceid"])
                    {
                        continue;
                    }

                    MusicResultDataItem result;
                    result.m_name = value["name"].toString();
                    result.m_coverUrl = value["pic"].toString();
                    result.m_count = value["id"].toString();
                    result.m_description = value["intro"].toString();
                    result.m_updateTime = value["pub"].toString();
                    Q_EMIT createToplistItem(result);
                    return;
                }
            }
        }
    }
}
