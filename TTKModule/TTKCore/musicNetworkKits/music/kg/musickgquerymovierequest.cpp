#include "musickgquerymovierequest.h"

namespace ReqKGInterface
{
    /*!
     * Read mv infos from query results.
     */
    static void parseFromMovieInfo(TTK::MusicSongInformation *info);
    /*!
     * Read mv tags(size and bitrate and url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, bool more);
    /*!
     * Read mv tags(size and bitrate and url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key);

}

void ReqKGInterface::parseFromMovieInfo(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_URL, false).arg(info->m_songId));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    const QString text(bytes);

    static TTKRegularExpression regx1("mv_hash\\s?=\\s?\"([^\"]+)");
    if(regx1.match(text) != -1)
    {
        info->m_songId = regx1.captured(1);
    }

    static TTKRegularExpression regx2("mv_pic\\s?=\\s?\"([^\"]+)");
    if(regx2.match(text) != -1)
    {
        info->m_coverUrl = regx2.captured(1);
    }
}

void ReqKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, bool more)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &key = TTK::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(key, info->m_songId));
    ReqKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value.contains("mvdata"))
        {
            if(more)
            {
                info->m_songName = TTK::String::charactersReplace(value["songname"].toString());
                info->m_artistName = ReqKGInterface::makeSongArtist(value["singer"].toString());
            }

            value = value["mvdata"].toMap();

            QVariantMap mv = value["sd"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }

            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                parseFromMovieProperty(info, mv);
            }
        }
    }
}

void ReqKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    TTK::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = TTK::Number::sizeByteToLabel(key["filesize"].toInt());
    prop.m_format = TTK::String::splitToken(prop.m_url);

    const int bitrate = key["bitrate"].toInt() / 1000;
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

    if(info->m_duration.isEmpty())
    {
        info->m_duration = TTKTime::formatDuration(key["timelength"].toInt());
    }
    info->m_songProps.append(prop);
}


MusicKGQueryMovieRequest::MusicKGQueryMovieRequest(QObject *parent)
    : MusicUnityQueryMovieRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    if(needToUnity())
    {
        MusicUnityQueryMovieRequest::startToPage(offset);
        return;
    }

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryMovieRequest::startToSearch(const QString &value)
{
    resetUnity();
    MusicUnityQueryMovieRequest::startToSearch(value);
}

void MusicKGQueryMovieRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << value);

    deleteAll();
    m_queryValue = value;

    TTK_SIGNLE_SHOT(downloadSingleFinished, TTK_SLOT);
}

void MusicKGQueryMovieRequest::downloadFinished()
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
                    info.m_songId = value["mvhash"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["songname"].toString());

                    info.m_artistName = ReqKGInterface::makeSongArtist(value["singername"].toString());

                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKGInterface::parseFromMovieProperty(&info, false);
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
                    }

                    Q_EMIT createResultItem({info, serverToString()});
                    m_items << info;
                }
            }
        }
    }

    if(!pageValid())
    {
        setToUnity();
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}

void MusicKGQueryMovieRequest::downloadSingleFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicQueryMovieRequest::downloadFinished();

    TTK::MusicSongInformation info;
    info.m_songId = m_queryValue;

    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromMovieInfo(&info);
    TTK_NETWORK_QUERY_CHECK();
    ReqKGInterface::parseFromMovieProperty(&info, true);
    TTK_NETWORK_QUERY_CHECK();

    if(!info.m_songProps.isEmpty())
    {
        Q_EMIT createResultItem({info, serverToString()});
        m_items << info;
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}



MusicKGQueryArtistMovieRequest::MusicKGQueryArtistMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = TTK_PAGE_SIZE_12;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryArtistMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__ << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_MOVIE_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    ReqKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryArtistMovieRequest::downloadFinished()
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
            if(value.contains("data") && value["errcode"].toInt() == 0)
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

                    MusicResultDataItem item;
                    item.m_id = value["hash"].toString();
                    item.m_coverUrl = value["imgurl"].toString();
                    item.m_name = value["filename"].toString();
                    item.m_time.clear();
                    Q_EMIT createMovieItem(item);
                }
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}
