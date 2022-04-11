#include "musickgqueryplaylistrequest.h"

MusicKGQueryPlaylistRequest::MusicKGQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryPlaylistRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == MusicQuery)
    {
        startToSearch(value);
    }
    else
    {
        m_queryValue = value;
        startToPage(0);
    }
}

void MusicKGQueryPlaylistRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_PLAYLIST_URL, false).arg(m_queryValue).arg(offset + 1).arg(m_pageSize));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryPlaylistRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_PLAYLIST_INFO_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(finished()), SLOT(downloadDetailsFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryPlaylistRequest::queryPlaylistInfo(MusicResultsItem &item)
{
    TTK_LOGGER_INFO(QString("%1 queryPlaylistInfo %2").arg(className(), item.m_id));

    MusicPageQueryRequest::downLoadFinished();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_PLAYLIST_DETAIL_URL, false).arg(item.m_id));
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
        if(value["errcode"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            item.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
            item.m_name = value["specialname"].toString();
            item.m_playCount = QString::number(value["playcount"].toLongLong());
            item.m_description = value["intro"].toString();
            item.m_updateTime = value["publishtime"].toString();
            item.m_nickName = value["nickname"].toString();

            item.m_tags.clear();
            const QVariantList &tags = value["tags"].toList();
            for(const QVariant &var : qAsConst(tags))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                item.m_tags.append(value["tagname"].toString() + "|");
            }
        }
    }
}

void MusicKGQueryPlaylistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryPlaylistRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["errcode"].toInt() == 0)
            {
                value = value["data"].toMap();
                m_totalSize = value["total"].toLongLong();

                const QVariantList &datas = value["info"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_coverUrl = value["imgurl"].toString().replace("{size}", "400");
                    item.m_id = QString::number(value["specialid"].toULongLong());
                    item.m_name = value["specialname"].toString();
                    item.m_playCount = value["playcount"].toString();
                    item.m_description = value["intro"].toString();
                    item.m_updateTime = value["publishtime"].toString();
                    item.m_tags = TTK_DEFAULT_STR;
                    item.m_nickName = value["username"].toString();

                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicKGQueryPlaylistRequest::downloadDetailsFinished()
{
    TTK_LOGGER_INFO(QString("%1 downloadDetailsFinished").arg(className()));

    MusicQueryPlaylistRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
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

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["filename"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt() * 1000);

                    if(musicInfo.m_songName.contains(TTK_DEFAULT_STR))
                    {
                        const QStringList &ll = musicInfo.m_songName.split(TTK_DEFAULT_STR);
                        musicInfo.m_singerName = MusicUtils::String::charactersReplaced(ll.front().trimmed());
                        musicInfo.m_songName = MusicUtils::String::charactersReplaced(ll.back().trimmed());
                    }

                    musicInfo.m_songId = value["hash"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "1";
                    musicInfo.m_trackNumber = "0";

                    MusicResultsItem albumInfo;
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongAlbumInfo(&albumInfo, musicInfo.m_albumId);
                    musicInfo.m_albumName = albumInfo.m_nickName;
                    TTK_NETWORK_QUERY_CHECK();

                    readFromMusicSongLrcAndPicture(&musicInfo);
                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongProperty(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_duration = musicInfo.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
}
