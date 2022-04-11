#include "musickgquerytoplistrequest.h"

MusicKGQueryToplistRequest::MusicKGQueryToplistRequest(QObject *parent)
    : MusicQueryToplistRequest(parent)
{
    m_queryServer = QUERY_KG_INTERFACE;
}

void MusicKGQueryToplistRequest::startToSearch(QueryType type, const QString &value)
{
    if(type == MusicQuery)
    {
        startToSearch(value);
    }
    else
    {
        startToSearch(value.isEmpty() ? "6666" : value);
    }
}

void MusicKGQueryToplistRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_TOPLIST_URL, false).arg(value));
    MusicKGInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicKGQueryToplistRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryToplistRequest::downLoadFinished();
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
                if(!initialize())
                {
                    return;
                }
                //
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
    deleteAll();
}

bool MusicKGQueryToplistRequest::initialize()
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_TOPLIST_INFO_URL, false));
    MusicKGInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return false;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            MusicResultsItem info;
            info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["timestamp"].toLongLong() * 1000).toString(MUSIC_YEAR_FORMAT);

            const QVariantList &datas = value["info"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                if(m_queryValue != value["rankid"])
                {
                    continue;
                }

                info.m_name = value["rankname"].toString();
                info.m_coverUrl = value["banner7url"].toString().replace("{size}", "400");
                info.m_playCount = value["play_times"].toString();
                info.m_description = value["intro"].toString();
                Q_EMIT createToplistInfoItem(info);
                return true;
            }
        }
    }
    return true;
}
