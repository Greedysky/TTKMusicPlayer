#include "musicdownloadquerykwplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWPlaylistThread::MusicDownLoadQueryKWPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 30;
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWPlaylistThread::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist.isEmpty() ? "167" : playlist;
        startToPage(0);
    }
}

void MusicDownLoadQueryKWPlaylistThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_PLAYLIST_URL, false)
                    .arg(m_searchText).arg(offset).arg(m_pageSize);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWPlaylistThread::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(playlist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_PLAYLIST_ATTR_URL, false).arg(playlist);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWPlaylistThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            m_pageTotal = value["total"].toString().toLongLong();
            if(value.contains("child"))
            {
                m_tags = value["ninfo"].toMap()["name"].toString();
                QVariantList datas = value["child"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    getMorePlaylistDetails(value["sourceid"].toString());
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKWPlaylistThread::getDetailsFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    M_LOGGER_INFO(QString("%1 getDetailsFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(reply && m_manager && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(!value.isEmpty() && value.contains("musiclist"))
            {
                QVariantList datas = value["musiclist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["artist"].toString();
                    musicInfo.m_songName = value["name"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    musicInfo.m_songId = value["id"].toString();
                    musicInfo.m_artistId = value["artistid"].toString();
                    musicInfo.m_albumId = value["albumid"].toString();

                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
//                    readFromMusicSongPic(&musicInfo, musicInfo.m_songId, m_manager);
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(musicInfo.m_songId);
                    ///music normal songs urls
                    readFromMusicSongAttribute(&musicInfo, value["formats"].toString(), m_searchQuality, m_queryAllRecords);
                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songname = musicInfo.m_songName;
                    item.m_artistname = musicInfo.m_singerName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    M_LOGGER_INFO(QString("%1 getDetailsFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKWPlaylistThread::getMorePlaylistDetailsFinished()
{
    M_LOGGER_INFO(QString("%1 getMorePlaylistDetailsFinished").arg(getClassName()));
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["result"].toString() == "ok")
            {
                MusicPlaylistItem item;
                item.m_tags = m_tags;
                item.m_coverUrl = value["pic"].toString();
                item.m_id = value["id"].toString();
                item.m_name = value["title"].toString();
                item.m_playCount = QString::number(value["playnum"].toULongLong());
                item.m_description = value["info"].toString();
                item.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["ctime"].toULongLong()*1000).toString("yyyy-MM-dd");
                item.m_nickname = value["uname"].toString();
                emit createPlaylistItems(item);
            }
        }
    }
    M_LOGGER_INFO(QString("%1 getMorePlaylistDetailsFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKWPlaylistThread::getMorePlaylistDetails(const QString &pid)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 getMorePlaylistDetails %2").arg(getClassName()).arg(pid));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_PLAYLIST_ATTR_URL, false).arg(pid);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getMorePlaylistDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
