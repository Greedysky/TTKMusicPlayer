#include "musickwquerymovierequest.h"

#include "qalgorithm/deswrapper.h"

namespace MusicKWInterface
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

void MusicKWInterface::parseFromMovieInfo(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    info->m_songName = "Not Found";
    info->m_singerName = "Anonymous";

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(info->m_songId));
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
            value = value["data"].toMap();
            info->m_songName = value["name"].toString();
            info->m_singerName = value["artist"].toString();
            info->m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);
        }
    }
}

void MusicKWInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format)
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

void MusicKWInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &format, int bitrate)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QAlgorithm::Des des;
    const QByteArray &parameter = des.encrypt(TTK::Algorithm::mdII(KW_MOVIE_ATTR_URL, false).arg(info->m_songId, format).toUtf8(),
                                              TTK::Algorithm::mdII(_SIGN, ALG_SHR_KEY, false).toUtf8());
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_MOVIE_URL, false).arg(QString(parameter)));
    MusicKWInterface::makeRequestRawHeader(&request);

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
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = 40;
    m_queryServer = QUERY_KW_INTERFACE;
}

void MusicKWQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageSize = 20;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_ARTIST_MOVIE_URL, false).arg(m_queryValue).arg(m_pageSize).arg(offset));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadPageFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryMovieRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value.trimmed();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(value).arg(0).arg(m_pageSize));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKWQueryMovieRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSingleSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value.trimmed();

    TTK_SIGNLE_SHOT(downLoadSingleFinished, TTK_SLOT);
}

void MusicKWQueryMovieRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryMovieRequest::downLoadFinished();
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
                    info.m_singerName = TTK::String::charactersReplace(value["ARTIST"].toString());
                    info.m_songName = TTK::String::charactersReplace(value["SONGNAME"].toString());
                    info.m_duration = TTKTime::formatDuration(value["DURATION"].toInt() * TTK_DN_S2MS);

                    info.m_songId = value["MUSICRID"].toString().remove("MUSIC_");
                    TTK_NETWORK_QUERY_CHECK();
                    MusicKWInterface::parseFromMovieProperty(&info, value["FORMATS"].toString());
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
                    }

                    if(!findUrlFileSize(&info.m_songProps, info.m_duration))
                    {
                        return;
                    }

                    MusicResultInfoItem item;
                    item.m_songName = info.m_songName;
                    item.m_singerName = info.m_singerName;
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

void MusicKWQueryMovieRequest::downLoadPageFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadPageFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll().replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            m_totalSize = value["total"].toLongLong();
            if(value.contains("mvlist"))
            {
                const QVariantList &datas = value["mvlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_id = value["musicid"].toString();
                    result.m_coverUrl = value["pic"].toString();
                    if(!TTK::String::isNetworkUrl(result.m_coverUrl))
                    {
                        result.m_coverUrl = TTK::Algorithm::mdII(KW_MOVIE_COVER_URL, false) + result.m_coverUrl;
                    }
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

void MusicKWQueryMovieRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadSingleFinished").arg(className()));

    MusicQueryMovieRequest::downLoadFinished();

    TTK::MusicSongInformation info;
    info.m_songId = m_queryValue;
    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromMovieInfo(&info);
    TTK_NETWORK_QUERY_CHECK();
    MusicKWInterface::parseFromMovieProperty(&info, "MP4UL|MP4L|MP4HV|MP4");
    TTK_NETWORK_QUERY_CHECK();

    if(!info.m_songProps.isEmpty())
    {
        if(!findUrlFileSize(&info.m_songProps, info.m_duration))
        {
            return;
        }

        MusicResultInfoItem item;
        item.m_songName = info.m_songName;
        item.m_singerName = info.m_singerName;
        item.m_duration = info.m_duration;
        item.m_type = mapQueryServerString();
        Q_EMIT createSearchedItem(item);
        m_songInfos << info;
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
