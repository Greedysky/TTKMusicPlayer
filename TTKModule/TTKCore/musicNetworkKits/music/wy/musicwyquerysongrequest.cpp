#include "musicwyquerysongrequest.h"

static constexpr const char *WY_SONG_INFO_URL = "S3paVUxRY3hZVDVweWhJZFU0K0FpakhJc3R5YW1hbmpmRUVsL3JTRUdwcnlDdDlMWFJSR0ludFFPZ1MycWExVVlLNjg5Njh3VmV2cGxpQzE=";
static constexpr const char *WY_SONG_INFO_DATA_URL = "Zmh6cVNyd2REdlVlZ3JQVUZpNjd5YlBaYVR0SEQrWFAydVpzOWc9PQ==";
static constexpr const char *WY_NEW_SONG_URL = "SnJhQ3VyZzNwU0ZnYlo2ME9BRVFjdmp0aGNTTldyL2c2RmtES3dFSkY4b0NFZGUwWDRteFpDS0F0SUozOU9nR0RVVE9lTUFwMEhNPQ==";
static constexpr const char *WY_NEW_SONG_DATA_URL = "UnNXcXM0ODFxamFPVmFWeEM3Tm0xL1dZNlZTdDBib3QzejhwUXlsNEg4OGI2aDl4";

MusicWYQuerySongRequest::MusicWYQuerySongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_30;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQuerySongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SEARCH_URL, false),
                      TTK::Algorithm::mdII(WY_SEARCH_DATA_URL, false).arg(1).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize).toUtf8());

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQuerySongRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_INFO_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downloadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicWYQuerySongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicQuerySongRequest::startToQueryResult(info, bitrate);
}

void MusicWYQuerySongRequest::downloadFinished()
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
            if(value["code"].toInt() == 200)
            {
                value = value["result"].toMap();
                m_totalSize = value["songCount"].toInt();

                const QVariantList &datas = value["songs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = ReqWYInterface::makeCoverPixmapUrl(albumObject["pic_str"].toString());
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
                    info.m_trackNumber = value["no"].toString();

                    if(m_queryMode != QueryMode::Meta)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        ReqWYInterface::parseFromSongProperty(&info, value);
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

void MusicWYQuerySongRequest::downloadSingleFinished()
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
            if(value["code"].toInt() == 200 && value.contains("songs"))
            {
                const QVariantList &datas = value["songs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = ReqWYInterface::makeCoverPixmapUrl(albumObject["pic_str"].toString());
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
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



MusicWYQueryNewSongRequest::MusicWYQueryNewSongRequest(QObject *parent)
    : MusicQuerySongRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
    m_pageSize = 1;
    m_totalSize = 13;
}

void MusicWYQueryNewSongRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                       TTK::Algorithm::mdII(WY_NEW_SONG_URL, false),
                       TTK::Algorithm::mdII(WY_NEW_SONG_DATA_URL, false).arg(m_queryValue));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryNewSongRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downloadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();

    fetchUrlPathSize(&info->m_songProps, info->m_duration);
    MusicAbstractQueryRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryNewSongRequest::downloadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("data"))
            {
                value = value["data"].toMap();

                const QVariantList &datas = value["newList"].toList();
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
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["al"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = ReqWYInterface::makeCoverPixmapUrl(albumObject["pic_str"].toString());
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year = TTKDateTime::format(value["publishTime"].toULongLong(), TTK_YEAR_FORMAT);
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, value);
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
