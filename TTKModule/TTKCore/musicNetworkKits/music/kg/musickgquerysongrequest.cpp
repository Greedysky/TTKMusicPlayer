#include "musickgquerysongrequest.h"

static constexpr const char *KG_NEW_SONG_URL = "aTdvazN1ZzN3cWFqc2VlZzlNSE9GZW1Mdks2VkJlTlR2cVBMV2hud0ZPL2pURWZPSkVZUmxIQ1NuZ0ZYS0lzc0pwZE5ENGxFSEU4WVVoRlFrUGdIYTNwdVg4WlJlOWlQNnBLLzV0QW1VcE9ZUXV3TEFiREVzbGFmSjRFUnZxN2ZXOEFsSUJGcW4zOD0=";

MusicKGQuerySongRequest::MusicKGQuerySongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQuerySongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SEARCH_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQuerySongRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(value));
    ReqKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicKGQuerySongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQuerySongRequest::startToQueryResult(info, bitrate);
}

void MusicKGQuerySongRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicPageQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toInt();

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
                    info.m_songId = value["hash"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["songname"].toString());

                    info.m_artistName = ReqKGInterface::makeSongArtist(value["singername"].toString());

                    info.m_albumId = value["album_id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["album_name"].toString());

                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year.clear();
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongAlbumLrc(&info);
                    TTK_NETWORK_QUERY_CHECK();

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        ReqKGInterface::parseFromSongProperty(&info, value);
                        TTK_NETWORK_QUERY_CHECK();

                        Q_EMIT createResultItem({info, serverToString()});
                    }

                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}

void MusicKGQuerySongRequest::downloadSingleFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQuerySongRequest::downloadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["errcode"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();

                TTK::MusicSongInformation info;
                info.m_songId = value["hash"].toString();
                info.m_songName = TTK::String::charactersReplace(value["songname"].toString());

                info.m_artistId = value["singerid"].toString();
                info.m_artistName = ReqKGInterface::makeSongArtist(value["singername"].toString());

                const QVariantList &albumArray = value["album"].toList();
                for(const QVariant &var : qAsConst(albumArray))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    const QVariantMap &albumObject = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongAlbumInfo(&info, albumObject["album_audio_id"].toString());
                    TTK_NETWORK_QUERY_CHECK();
                }

                info.m_coverUrl = value["imgurl"].toString().replace("{size}", "480");
                info.m_lrcUrl = TTK::Algorithm::mdII(KG_SONG_LRC_URL, false).arg(info.m_songName, info.m_songId).arg(value["duration"].toInt() * TTK_DN_S2MS);
                info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                info.m_year.clear();
                info.m_trackNumber = "0";

                TTK_NETWORK_QUERY_CHECK();
                ReqKGInterface::parseFromSongProperty(&info, value["extra"].toMap());
                TTK_NETWORK_QUERY_CHECK();

                Q_EMIT createResultItem({info, serverToString()});
                m_items << info;
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGQueryNewSongRequest::MusicKGQueryNewSongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
    m_pageSize = 1;
    m_totalSize = 13;
}

void MusicKGQueryNewSongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_NEW_SONG_URL, false).arg(m_totalSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryNewSongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicKGQueryNewSongRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractQueryRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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

                    if(!pageValid())
                    {
                        break;
                    }

                    ++m_pageIndex;

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["hash"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["songname"].toString());

                    const QVariantList &artistsArray = value["authors"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["author_id"].toString();
                        }

                        info.m_artistName = ReqKGInterface::makeSongArtist(info.m_artistName, artistObject["author_name"].toString());
                    }

                    info.m_albumId = value["album_id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(value["remark"].toString());

                    info.m_coverUrl = value["cover"].toString().replace("{size}", "480");
                    info.m_lrcUrl = TTK::Algorithm::mdII(KG_SONG_LRC_URL, false).arg(info.m_songName, info.m_songId).arg(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
                    info.m_year = TTKDateTime::format(value["addtime"].toULongLong(), TTK_YEAR_FORMAT);
                    info.m_trackNumber = "0";

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromSongProperty(&info, value);
                    TTK_NETWORK_QUERY_CHECK();

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
