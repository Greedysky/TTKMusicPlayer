#include "musickgquerymovierequest.h"

namespace MusicKGInterface
{
    /*!
     * Read mv infos from query results.
     */
    static void parseFromMovieInfo(TTK::MusicSongInformation *info);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, bool more);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    static void parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key);

}

void MusicKGInterface::parseFromMovieInfo(TTK::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_URL, false).arg(info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    const QString text(bytes);
    const QRegExp regx("mv_hash = \"([^\"]+)");

    if(regx.indexIn(text) != -1)
    {
        info->m_songId = regx.cap(1);
    }
}

void MusicKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, bool more)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = TTK::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(encodedData, info->m_songId));
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
        if(!value.isEmpty() && value.contains("mvdata"))
        {
            if(more)
            {
                info->m_songName = value["songname"].toString();
                info->m_singerName = value["singer"].toString();
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

void MusicKGInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QVariantMap &key)
{
    TTK::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = TTK::Number::sizeByteToLabel(key["filesize"].toInt());
    prop.m_format = TTK::String::slitToken(prop.m_url);

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
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryMovieRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(className() << "startToPage" << offset);

    deleteAll();
    m_totalSize = 0;
    m_pageSize = 20;

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_ARTIST_MOVIE_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadPageFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryMovieRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSearch" << value);

    deleteAll();
    m_queryValue = value.trimmed();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(value).arg(0).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicKGQueryMovieRequest::startToSingleSearch(const QString &value)
{
    TTK_INFO_STREAM(className() << "startToSingleSearch" << value);

    deleteAll();
    m_queryValue = value.trimmed();

    TTK_SIGNLE_SHOT(downLoadSingleFinished, TTK_SLOT);
}

void MusicKGQueryMovieRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicQueryMovieRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
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
                    info.m_singerName = TTK::String::charactersReplace(value["singername"].toString());
                    info.m_songName = TTK::String::charactersReplace(value["songname"].toString());
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt() * TTK_DN_S2MS);

                    info.m_songId = value["mvhash"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    MusicKGInterface::parseFromMovieProperty(&info, false);
                    TTK_NETWORK_QUERY_CHECK();

                    if(info.m_songProps.isEmpty())
                    {
                        continue;
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

void MusicKGQueryMovieRequest::downLoadPageFinished()
{
    TTK_INFO_STREAM(className() << "downLoadPageFinished");

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
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

                    MusicResultDataItem result;
                    result.m_id = value["hash"].toString();
                    result.m_coverUrl = value["imgurl"].toString();
                    result.m_name = value["filename"].toString();
                    result.m_updateTime.clear();
                    Q_EMIT createMovieItem(result);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicKGQueryMovieRequest::downLoadSingleFinished()
{
    TTK_INFO_STREAM(className() << "downLoadSingleFinished");

    MusicQueryMovieRequest::downLoadFinished();

    TTK::MusicSongInformation info;
    info.m_songId = m_queryValue;
    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromMovieInfo(&info);
    TTK_NETWORK_QUERY_CHECK();
    MusicKGInterface::parseFromMovieProperty(&info, true);
    TTK_NETWORK_QUERY_CHECK();

    if(!info.m_songProps.isEmpty())
    {
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
