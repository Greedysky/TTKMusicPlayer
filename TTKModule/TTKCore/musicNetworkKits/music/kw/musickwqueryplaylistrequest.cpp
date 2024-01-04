#include "musickwqueryplaylistrequest.h"

MusicKWQueryPlaylistRequest::MusicKWQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryPlaylistRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(value));
    MusicKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryPlaylistRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSingleSearch" << value);

    m_queryValue = value.isEmpty() ? "167" : value;
    startToPage(0);
}

void MusicKWQueryPlaylistRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    findUrlFileSize(&info->m_songProps, info->m_duration);
    MusicQueryPlaylistRequest::startToQueryResult(info, bitrate);
}

void MusicKWQueryPlaylistRequest::startToQueryInfo(MusicResultDataItem &item)
{
    TTK_INFO_STREAM(className() << "startToQueryInfo" << item.m_id);

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(item.m_id));
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
        const QVariantMap &value = data.toMap();
        if(!value.isEmpty())
        {
            item.m_coverUrl = value["pic"].toString();
            item.m_name = value["title"].toString();
            item.m_count = value["playnum"].toString();
            item.m_description = value["info"].toString();
            item.m_updateTime = TTKDateTime::format(value["ctime"].toULongLong() * TTK_DN_S2MS, TTK_YEAR_FORMAT);
            item.m_nickName = value["uname"].toString();
        }
    }
}

void MusicKWQueryPlaylistRequest::downLoadFinished()
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
            if(value.contains("child"))
            {
                m_totalSize = value["total"].toInt();
                m_tags = value["ninfo"].toMap()["name"].toString();

                const QVariantList &datas = value["child"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    morePlaylistDetails(value["sourceid"].toString());
                    TTK_NETWORK_QUERY_CHECK();
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryPlaylistRequest::downloadDetailsFinished()
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
            if(value.contains("musiclist"))
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
                    item.m_type = serverToString();
                    Q_EMIT createSearchedItem(item);
                    m_songInfos << info;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
}

void MusicKWQueryPlaylistRequest::downloadMoreDetailsFinished()
{
    TTK_INFO_STREAM(className() << "downloadMoreDetailsFinished");

    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            if(value["result"].toString() == "ok")
            {
                MusicResultDataItem result;
                result.m_category = m_tags;
                result.m_coverUrl = value["pic"].toString();
                result.m_id = value["id"].toString();
                result.m_name = value["title"].toString();
                result.m_count = value["playnum"].toString();
                result.m_description = value["info"].toString();
                result.m_updateTime = TTKDateTime::format(value["ctime"].toULongLong() * TTK_DN_S2MS, TTK_YEAR_FORMAT);
                result.m_nickName = value["uname"].toString();
                Q_EMIT createPlaylistItem(result);
            }
        }
    }
}

void MusicKWQueryPlaylistRequest::morePlaylistDetails(const QString &pid)
{
    TTK_INFO_STREAM(className() << "morePlaylistDetails" << pid);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_PLAYLIST_INFO_URL, false).arg(pid));
    MusicKWInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadMoreDetailsFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}
