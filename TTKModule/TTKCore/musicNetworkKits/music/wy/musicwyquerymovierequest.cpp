#include "musicwyquerymovierequest.h"

namespace ReqWYInterface
{
    /*!
     * Start to get video url path.
     */
    static void parseFromMovieProperty(QString &url, const QString &id, int bitrate);

}

void ReqWYInterface::parseFromMovieProperty(QString &url, const QString &id, int bitrate)
{
    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_VIDEO_PATH_URL, false),
                      TTK::Algorithm::mdII(WY_VIDEO_PATH_DATA_URL, false).arg(id).arg(bitrate));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
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
        if(value["code"].toInt() == 200)
        {
            const QVariantList &datas = value["urls"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                url = value["url"].toString();

                if(!url.isEmpty())
                {
                    return;
                }
            }
        }
    }
}


MusicWYQueryMovieRequest::MusicWYQueryMovieRequest(QObject *parent)
    : MusicUnityQueryMovieRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    if(needToUnity())
    {
        MusicUnityQueryMovieRequest::startToPage(offset);
        return;
    }

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SONG_SEARCH_URL, false),
                      TTK::Algorithm::mdII(WY_SONG_SEARCH_DATA_URL, false).arg(1014).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize).toUtf8());

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryMovieRequest::startToSearch(const QString &value)
{
    resetUnity();
    MusicUnityQueryMovieRequest::startToSearch(value);
}

void MusicWYQueryMovieRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearchByID" << value);

    deleteAll();
    m_queryValue = value;

    TTK_SIGNLE_SHOT(downLoadSingleFinished, TTK_SLOT);
}

void MusicWYQueryMovieRequest::downLoadFinished()
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
                m_totalSize = value["videoCount"].toInt();

                const QVariantList &datas = value["videos"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    const int type = value["type"].toInt();
                    if(type == 0)
                    {
                        const qint64 vid = value["vid"].toLongLong();
                        if(vid == 0)
                        {
                            continue;
                        }

                        TTK_NETWORK_QUERY_CHECK();
                        parseFromMovieList(vid);
                        TTK_NETWORK_QUERY_CHECK();
                    }
                    else if(type == 1)
                    {
                        TTK_NETWORK_QUERY_CHECK();
                        parseFromVideoList(value["vid"].toString());
                        TTK_NETWORK_QUERY_CHECK();
                    }
                }
            }
        }
    }

    if(!pageValid())
    {
        setToUnity();
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryMovieRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(className() << "downLoadSingleFinished");

    MusicQueryMovieRequest::downLoadFinished();

    const qint64 vid = m_queryValue.toLongLong();
    if(vid != 0)
    {
        TTK_NETWORK_QUERY_CHECK();
        parseFromMovieList(vid);
        TTK_NETWORK_QUERY_CHECK();
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryMovieRequest::parseFromMovieList(qint64 id)
{
    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_MOVIE_URL, false),
                      TTK::Algorithm::mdII(WY_MOVIE_DATA_URL, false).arg(id));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
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
        if(value["code"].toInt() == 200)
        {
            value = value["data"].toMap();
            TTK_NETWORK_QUERY_CHECK();

            TTK::MusicSongInformation info;
            info.m_songId = QString::number(id);
            info.m_songName = TTK::String::charactersReplace(value["name"].toString());

            info.m_artistName = TTK::String::charactersReplace(value["artistName"].toString());

            info.m_duration = TTKTime::formatDuration(value["duration"].toInt());

            value = value["brs"].toMap();
            for(const QString &key : value.keys())
            {
                const int bitrate = key.toInt();
                TTK::MusicSongProperty prop;
                if(bitrate <= 375)
                {
                    prop.m_bitrate = TTK_BN_250;
                }
                else if(bitrate > 375 && bitrate <= 625)
                {
                    prop.m_bitrate = TTK_BN_500;
                }
                else if(bitrate > 625 && bitrate <= 875)
                {
                    prop.m_bitrate = TTK_BN_750;
                }
                else if(bitrate > 875)
                {
                    prop.m_bitrate = TTK_BN_1000;
                }

                prop.m_url = value[key].toString();
                prop.m_format = TTK::String::splitToken(prop.m_url);

                if(!findUrlPathSize(&prop, info.m_duration))
                {
                    return;
                }

                info.m_songProps.append(prop);
            }

            if(info.m_songProps.isEmpty())
            {
                return;
            }

            Q_EMIT createResultItem({info, serverToString()});
            m_songInfos << info;
        }
    }
}

void MusicWYQueryMovieRequest::parseFromVideoList(const QString &id)
{
    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_VIDEO_INFO_URL, false),
                      TTK::Algorithm::mdII(WY_VIDEO_INFO_DATA_URL, false).arg(id));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
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
        if(value["code"].toInt() == 200)
        {
            value = value["data"].toMap();
            TTK_NETWORK_QUERY_CHECK();

            TTK::MusicSongInformation info;
            info.m_songId = id;
            info.m_songName = TTK::String::charactersReplace(value["title"].toString());

            const QVariantMap &artistObject = value["creator"].toMap();
            info.m_artistName = TTK::String::charactersReplace(artistObject["nickname"].toString());

            info.m_duration = TTKTime::formatDuration(value["durationms"].toInt());

            const QVariantList &datas = value["resolutions"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                const int bitrate = value["resolution"].toInt();
                TTK::MusicSongProperty prop;
                ReqWYInterface::parseFromMovieProperty(prop.m_url, id, bitrate);
                TTK_NETWORK_QUERY_CHECK();

                if(prop.isEmpty())
                {
                    continue;
                }

                if(bitrate <= 375)
                {
                    prop.m_bitrate = TTK_BN_250;
                }
                else if(bitrate > 375 && bitrate <= 625)
                {
                    prop.m_bitrate = TTK_BN_500;
                }
                else if(bitrate > 625 && bitrate <= 875)
                {
                    prop.m_bitrate = TTK_BN_750;
                }
                else if(bitrate > 875)
                {
                    prop.m_bitrate = TTK_BN_1000;
                }

                prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                prop.m_format = TTK::String::splitToken(prop.m_url);
                info.m_songProps.append(prop);
            }

            if(info.m_songProps.isEmpty())
            {
                return;
            }

            Q_EMIT createResultItem({info, serverToString()});
            m_songInfos << info;
        }
    }
}



MusicWYQueryArtistMovieRequest::MusicWYQueryArtistMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = ARTIST_ATTR_PAGE_SIZE;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_MOVIE_URL, false),
                      TTK::Algorithm::mdII(WY_ARTIST_MOVIE_DATA_URL, false).arg(m_queryValue).arg(m_pageSize * offset).arg(m_pageSize));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistMovieRequest::downLoadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("mvs"))
            {
                TTK_NETWORK_QUERY_CHECK();
                parseFromArtistMoviesCount(m_queryValue.toLongLong());
                TTK_NETWORK_QUERY_CHECK();

                const QVariantList &datas = value["mvs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_id = value["id"].toString();
                    result.m_coverUrl = value["imgurl"].toString();
                    result.m_name = value["name"].toString();
                    result.m_updateTime.clear();
                    Q_EMIT createMovieItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicWYQueryArtistMovieRequest::parseFromArtistMoviesCount(qint64 id)
{
    m_totalSize = TTK_HIGH_LEVEL;

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_MOVIE_URL, false),
                      TTK::Algorithm::mdII(WY_ARTIST_MOVIE_DATA_URL, false).arg(id).arg(0).arg(m_totalSize));

    const QByteArray &bytes = TTK::syncNetworkQueryForPost(&request, parameter);
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
        if(value["code"].toInt() == 200 && value.contains("mvs"))
        {
            m_totalSize = value["mvs"].toList().count();
        }
    }
}
