#include "musickgquerymovierequest.h"

MusicKGQueryMovieRequest::MusicKGQueryMovieRequest(QObject *parent)
    : MusicQueryMovieRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryMovieRequest::startToSearch(QueryType type, const QString &text)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), text));

    deleteAll();
    m_queryText = text.trimmed();
    m_currentType = type;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SONG_SEARCH_URL, false).arg(text).arg(0).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryMovieRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;
    m_pageSize = 20;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ARTIST_MOVIE_URL, false).arg(m_queryText).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadPageFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryMovieRequest::startToSingleSearch(const QString &text)
{
    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(className(), text));

    deleteAll();
    m_queryText = text.trimmed();

    QTimer::singleShot(MT_MS, this, SLOT(downLoadSingleFinished()));
}

void MusicKGQueryMovieRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryMovieRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
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

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::charactersReplaced(value["singername"].toString());
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["songname"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    musicInfo.m_songId = value["mvhash"].toString();
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicMVProperty(&musicInfo, false);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_duration = musicInfo.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKGQueryMovieRequest::downLoadPageFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadPageFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
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

                    MusicResultsItem info;
                    info.m_id = value["hash"].toString();
                    info.m_coverUrl = value["imgurl"].toString();
                    info.m_name = value["filename"].toString();
                    info.m_updateTime.clear();
                    Q_EMIT createMovieInfoItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKGQueryMovieRequest::downLoadSingleFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadSingleFinished").arg(className()));

    MusicQueryMovieRequest::downLoadFinished();

    MusicObject::MusicSongInformation musicInfo;
    musicInfo.m_songId = m_queryText;
    TTK_NETWORK_QUERY_CHECK();
    readFromMusicMVInfo(&musicInfo);
    TTK_NETWORK_QUERY_CHECK();
    readFromMusicMVProperty(&musicInfo, true);
    TTK_NETWORK_QUERY_CHECK();

    if(!musicInfo.m_songProps.isEmpty())
    {
        MusicSearchedItem item;
        item.m_songName = musicInfo.m_songName;
        item.m_singerName = musicInfo.m_singerName;
        item.m_duration = musicInfo.m_duration;
        item.m_type = mapQueryServerString();
        Q_EMIT createSearchedItem(item);
        m_musicSongInfos << musicInfo;
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKGQueryMovieRequest::readFromMusicMVInfo(MusicObject::MusicSongInformation *info) const
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_MOVIE_URL, false).arg(info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    const QString text(bytes);
    QRegExp regx("mv_hash = \"([^\"]+)");

    if(text.indexOf(regx) != -1)
    {
        info->m_songId = regx.cap(1);
    }
}

void MusicKGQueryMovieRequest::readFromMusicMVProperty(MusicObject::MusicSongInformation *info, bool more) const
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = MusicUtils::Algorithm::md5(QString("%1kugoumvcloud").arg(info->m_songId).toUtf8());

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_MOVIE_INFO_URL, false).arg(encodedData, info->m_songId));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok;
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
                readFromMusicMVProperty(info, mv);
            }
            mv = value["hd"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
            mv = value["sq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
            mv = value["rq"].toMap();
            if(!mv.isEmpty())
            {
                readFromMusicMVProperty(info, mv);
            }
        }
    }
}

void MusicKGQueryMovieRequest::readFromMusicMVProperty(MusicObject::MusicSongInformation *info, const QVariantMap &key) const
{
    MusicObject::MusicSongProperty prop;
    prop.m_url = key["downurl"].toString();
    prop.m_size = MusicUtils::Number::sizeByte2Label(key["filesize"].toInt());
    prop.m_format = MusicUtils::String::stringSplitToken(prop.m_url);

    const int bitrate = key["bitrate"].toInt() / 1000;
    if(bitrate <= 375)
        prop.m_bitrate = MB_250;
    else if(bitrate > 375 && bitrate <= 625)
        prop.m_bitrate = MB_500;
    else if(bitrate > 625 && bitrate <= 875)
        prop.m_bitrate = MB_750;
    else if(bitrate > 875)
        prop.m_bitrate = MB_1000;

    if(info->m_duration.isEmpty())
    {
        info->m_duration = MusicTime::msecTime2LabelJustified(key["timelength"].toInt());
    }
    info->m_songProps.append(prop);
}
