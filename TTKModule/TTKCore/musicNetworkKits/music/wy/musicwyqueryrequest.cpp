#include "musicwyqueryrequest.h"

MusicWYQueryRequest::MusicWYQueryRequest(QObject *parent)
    : MusicQueryRequest(parent)
{
    m_pageSize = SONG_PAGE_SIZE;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_SEARCH_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_SEARCH_DATA_URL, false).arg(1).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize).toUtf8());

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearchByID" << value);

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_INFO_DATA_URL, false).arg(value));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
    QtNetworkErrorConnect(reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    TTK_INFO_STREAM(className() << "startToQueryResult" << info->m_songId << bitrate << "kbps");

    MusicPageQueryRequest::downLoadFinished();
    TTK_NETWORK_QUERY_CHECK();
    ReqWYInterface::parseFromSongProperty(info, bitrate);
    TTK_NETWORK_QUERY_CHECK();
    MusicQueryRequest::startToQueryResult(info, bitrate);
}

void MusicWYQueryRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
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

                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year.clear();
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(className() << "downLoadSingleFinished");

    MusicQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObjectCast(QNetworkReply*, sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
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

                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["dt"].toInt());
                    info.m_year.clear();
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
