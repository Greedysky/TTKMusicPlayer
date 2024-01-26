#include "musickwquerymovierequest.h"

#include "qalgorithm/deswrapper.h"

namespace ReqKWInterface
{
    /*!
     * Read mv info property from query results.
     */
    static void parseFromMovieInfo(TTK::MusicSongInformation *info);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format, int bitrate);

}

void ReqKWInterface::parseFromMovieInfo(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    info->m_songName = "Not Found";
    info->m_artistName = "Anonymous";

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(info->m_songId));
    ReqKWInterface::makeRequestRawHeader(&request);

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
            value = value["data"].toMap();
            info->m_songName = TTK::String::charactersReplace(value["name"].toString());
            info->m_artistName = ReqKWInterface::makeSongArtist(value["artist"].toString());
            info->m_coverUrl = value["pic"].toString();
            info->m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
        }
    }
}

void ReqKWInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    for(const QString &v : format.split("|"))
    {
        if(v.contains("MP4L"))
        {
            parseFromMovieProperty(info, "MP4L", TTK_BN_250);
        }
        else if(v.contains("MP4HV"))
        {
            parseFromMovieProperty(info, "MP4HV", TTK_BN_750);
        }
        else if(v.contains("MP4UL"))
        {
            parseFromMovieProperty(info, "MP4UL", TTK_BN_1000);
        }
        else if(v.contains("MP4"))
        {
            parseFromMovieProperty(info, "MP4", TTK_BN_500);
        }
    }
}

void ReqKWInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QAlgorithm::Des des;
    const QByteArray &parameter = des.encrypt(TTK::Algorithm::mdII(KW_MOVIE_ATTR_URL, false).arg(info->m_songId, format).toUtf8(),
                                              TTK::Algorithm::mdII("OGlVTjJWOEdlMkkzSkZIeg==", ALG_SHR_KEY, false).toUtf8());
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_MOVIE_URL, false).arg(QString(parameter)));
    ReqKWInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    if(!bytes.isEmpty() && !bytes.contains("res not found"))
    {
        const QString text(bytes);
        const QRegExp regx(".*url=(.*)\r\nsig=");

        if(regx.indexIn(text) != -1)
        {
            TTK::MusicSongProperty prop;
            prop.m_url = regx.cap(1);
            prop.m_bitrate = bitrate;
            prop.m_format = MP4_FILE_SUFFIX;

            if(prop.isEmpty() || info->m_songProps.contains(prop))
            {
                return;
            }

            info->m_songProps.append(prop);
        }
    }
}


MusicKWQueryMovieRequest::MusicKWQueryMovieRequest(QObject *parent)
    : MusicUnityQueryMovieRequest(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryMovieRequest::startToPage(int offset)
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
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryMovieRequest::startToSearch(const QString &value)
{
    resetUnity();
    MusicUnityQueryMovieRequest::startToSearch(value);
}

void MusicKWQueryMovieRequest::startToSearchByID(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearchByID" << value);

    deleteAll();
    m_queryValue = value;

    TTK_SIGNLE_SHOT(downLoadSingleFinished, TTK_SLOT);
}

void MusicKWQueryMovieRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                m_totalSize = value["TOTAL"].toInt();

                const QVariantList &datas = value["abslist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["MUSICRID"].toString().remove("MUSIC_");
                    info.m_songName = TTK::String::charactersReplace(value["SONGNAME"].toString());

                    info.m_artistName = ReqKWInterface::makeSongArtist(value["ARTIST"].toString());

                    info.m_coverUrl = ReqKWInterface::makeCoverPixmapUrl(value["web_albumpic_short"].toString(), info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["DURATION"].toInt() * TTK_DN_S2MS);

                    TTK_NETWORK_QUERY_CHECK();
                    ReqKWInterface::parseFromMovieProperty(&info, value["FORMATS"].toString());
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty() || !findUrlPathSize(&info.m_songProps, info.m_duration))
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

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKWQueryMovieRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(className() << "downLoadSingleFinished");

    MusicQueryMovieRequest::downLoadFinished();

    TTK::MusicSongInformation info;
    info.m_songId = m_queryValue;

    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromMovieInfo(&info);
    TTK_NETWORK_QUERY_CHECK();
    ReqKWInterface::parseFromMovieProperty(&info, "MP4UL|MP4L|MP4HV|MP4");
    TTK_NETWORK_QUERY_CHECK();

    if(!info.m_songProps.isEmpty() && findUrlPathSize(&info.m_songProps, info.m_duration))
    {
        Q_EMIT createResultItem({info, serverToString()});
        m_items << info;
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}



MusicKWQueryArtistMovieRequest::MusicKWQueryArtistMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = ARTIST_ATTR_PAGE_SIZE;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryArtistMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageIndex = offset;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_MOVIE_URL, false).arg(m_queryValue).arg(offset).arg(m_pageSize));
    ReqKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryArtistMovieRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("mvlist"))
            {
                m_totalSize = value["total"].toInt();

                const QVariantList &datas = value["mvlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem item;
                    item.m_id = value["musicid"].toString();
                    item.m_coverUrl = value["pic"].toString();
                    if(!TTK::String::isNetworkUrl(item.m_coverUrl))
                    {
                        item.m_coverUrl = TTK::Algorithm::mdII(KW_MOVIE_COVER_URL, false) + item.m_coverUrl;
                    }
                    item.m_name = value["name"].toString();
                    item.m_updateTime.clear();
                    Q_EMIT createMovieItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
