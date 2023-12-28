#include "musickgqueryplaylistrequest.h"

MusicKGQueryPlaylistRequest::MusicKGQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryPlaylistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_PLAYLIST_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_PLAYLIST_INFO_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryPlaylistRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSingleSearch" << value);

    m_queryValue = value;
    startToPage(0);
}

void MusicKGQueryPlaylistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");
    MusicPageQueryRequest::downLoadFinished();

    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    MusicQueryPlaylistRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryPlaylistRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(className() << "startToQueryInfo" << item.m_id);

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_PLAYLIST_DETAIL_URL, false).arg(item.m_id));
    MusicKGInterface::makeRequestRawHeader(&request);

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
        if(value["errcode"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            item.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
            item.m_name = value["specialname"].toString();
            item.m_playCount = value["playcount"].toString();
            item.m_description = value["intro"].toString();
            item.m_updateTime = value["publishtime"].toString();
            item.m_nickName = value["nickname"].toString();
            item.m_tags.clear();

            const QVariantList &tags = value["tags"].toList();
            for(const QVariant &var : qAsConst(tags))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                item.m_tags.append(value["tagname"].toString() + "|");
            }
        }
    }
}

void MusicKGQueryPlaylistRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicQueryPlaylistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errcode"].toInt() == 0)
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toLongLong();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
                    result.m_id = value["specialid"].toString();
                    result.m_name = value["specialname"].toString();
                    result.m_playCount = value["playcount"].toString();
                    result.m_description = value["intro"].toString();
                    result.m_updateTime = value["publishtime"].toString();
                    result.m_tags = TTK_DEFAULT_STR;
                    result.m_nickName = value["username"].toString();
                    Q_EMIT createPlaylistItem(result);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKGQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_INFO_STREAM(className() << "downloadDetailsFinished");

    MusicQueryPlaylistRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songName = TTK::String::charactersReplace(value["filename"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    if(info.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = info.m_songName.split(TTK_DEFAULT_STR);
                        info.m_singerName = TTK::String::charactersReplace(ll.front().trimmed());
                        info.m_songName = TTK::String::charactersReplace(ll.back().trimmed());
                    }

                    info.m_songId = value["hash"].toString();
                    info.m_albumId = value["album_id"].toString();

                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    MusicResultDataItem albumInfo;
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongAlbumInfo(&albumInfo, info.m_albumId);
                    info.m_albumName = albumInfo.m_nickName;
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongLrcAndPicture(&info);
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromSongProperty(&info, value);
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
}
