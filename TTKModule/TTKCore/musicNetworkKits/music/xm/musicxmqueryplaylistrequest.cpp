#include "musicxmqueryplaylistrequest.h"
#include "musicsemaphoreloop.h"

MusicXMQueryPlaylistRequest::MusicXMQueryPlaylistRequest(QObject *parent)
    : MusicQueryPlaylistRequest(parent)
{
    m_pageSize = 30;
    m_queryServer = QUERY_XM_INTERFACE;
}

void MusicXMQueryPlaylistRequest::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist;
        startToPage(0);
    }
}

void MusicXMQueryPlaylistRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    makeTokenQueryUrl(&request, false,
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_DATA_URL, false).arg(m_searchText).arg(offset + 1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMQueryPlaylistRequest::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    makeTokenQueryUrl(&request, false,
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_INFO_DATA_URL, false).arg(playlist).arg(1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_INFO_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMQueryPlaylistRequest::getPlaylistInfo(MusicResultsItem &item)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 getPlaylistInfo %2").arg(getClassName()).arg(item.m_id));
    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    makeTokenQueryUrl(&request, false,
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_INFO_DATA_URL, false).arg(item.m_id).arg(1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_PLAYLIST_INFO_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    const QByteArray &bytes = reply->readAll();

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["data"].toMap();
            value = value["collectDetail"].toMap();

            item.m_coverUrl = value["collectLogo"].toString();
            item.m_name = value["collectName"].toString();
            item.m_playCount = QString::number(value["playCount"].toULongLong());
            item.m_description = value["description"].toString();
            item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["gmtModify"].toULongLong()).toString(MUSIC_YEAR_FORMAT);
            item.m_nickName = value["userName"].toString();

            const QVariantList &tags = value["tags"].toList();
            item.m_tags.clear();
            foreach(const QVariant &var, tags)
            {
                if(var.isNull())
                {
                    continue;
                }
                item.m_tags.append(var.toString() + "|");
            }
        }
    }
}

void MusicXMQueryPlaylistRequest::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                m_pageTotal = value["total"].toLongLong();

                const QVariantList &datas = value["collects"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem item;
                    item.m_coverUrl = value["collectLogo"].toString();
                    item.m_id = QString::number(value["listId"].toULongLong());
                    item.m_name = value["collectName"].toString();
                    item.m_playCount = QString::number(value["playCount"].toULongLong());
                    item.m_description = value["description"].toString();
                    item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["gmtModify"].toULongLong()).toString(MUSIC_YEAR_FORMAT);
                    item.m_nickName = value["userName"].toString();

                    const QVariantList &tags = value["tags"].toList();
                    item.m_tags.clear();
                    foreach(const QVariant &var, tags)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }
                        item.m_tags.append(var.toString() + "|");
                    }

                    Q_EMIT createPlaylistItem(item);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicXMQueryPlaylistRequest::getDetailsFinished()
{
    TTK_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());

    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(reply && m_manager && reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                value = value["collectDetail"].toMap();
                const QVariantList &datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["singers"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songName"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt());

                    musicInfo.m_songId = value["songId"].toString();
                    musicInfo.m_artistId = value["artistId"].toString();
                    musicInfo.m_albumId = value["albumId"].toString();
                    musicInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumName"].toString());

                    musicInfo.m_smallPicUrl = value["albumLogo"].toString();

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = "0";
                    musicInfo.m_trackNumber = value["track"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return;
                    readFromMusicSongAttribute(&musicInfo, value["listenFiles"], m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkQuery) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
}
