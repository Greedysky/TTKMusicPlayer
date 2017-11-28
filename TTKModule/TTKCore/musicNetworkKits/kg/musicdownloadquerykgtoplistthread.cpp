#include "musicdownloadquerykgtoplistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGToplistThread::MusicDownLoadQueryKGToplistThread(QObject *parent)
    : MusicDownLoadQueryToplistThread(parent)
{
    m_queryServer = "Kugou";
}

QString MusicDownLoadQueryKGToplistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKGToplistThread::startToSearch(QueryType type, const QString &toplist)
{
    if(type == MusicQuery)
    {
        startToSearch(toplist);
    }
    else
    {
        startToSearch(toplist.isEmpty() ? "6666" : toplist);
    }
}

void MusicDownLoadQueryKGToplistThread::startToSearch(const QString &toplist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_SONG_TOPLIST_URL, false).arg(toplist);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKGToplistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("songs"))
            {
                QVariantMap topInfo = value["info"].toMap();
                MusicPlaylistItem info;
                info.m_name = topInfo["rankname"].toString();
                info.m_coverUrl = topInfo["imgurl"].toString().replace("{size}", "400");
                info.m_playCount = "-";
                info.m_description = topInfo["intro"].toString();

                value = value["songs"].toMap();
                info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["timestamp"].toULongLong()*1000).toString("yyyy-MM-dd");

                emit createToplistInfoItem(info);
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    QVariantMap value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["filename"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    if(musicInfo.m_songName.contains("-"))
                    {
                        QStringList ll = musicInfo.m_songName.split("-");
                        musicInfo.m_singerName = ll.front().trimmed();
                        musicInfo.m_songName = ll.back().trimmed();
                    }

                    musicInfo.m_songId = value["hash"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    MusicPlaylistItem albumInfo;
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAlbumInfo(&albumInfo, musicInfo.m_albumId);
                    musicInfo.m_albumName = albumInfo.m_nickname;
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    readFromMusicSongLrcAndPic(&musicInfo, musicInfo.m_songId);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

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
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
