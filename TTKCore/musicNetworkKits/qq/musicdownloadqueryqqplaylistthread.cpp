#include "musicdownloadqueryqqplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQPlaylistThread::MusicDownLoadQueryQQPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 30;
    m_queryServer = "QQ";
}

QString MusicDownLoadQueryQQPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQPlaylistThread::startToSearch(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startToSearch(playlist);
    }
    else
    {
        m_searchText = playlist.isEmpty() ? "10000000" : playlist;
        startToPage(0);
    }
}

void MusicDownLoadQueryQQPlaylistThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_URL, false)
                    .arg(m_searchText).arg(m_pageSize*offset).arg(m_pageSize*(offset + 1) - 1);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Referer", "http://y.qq.com/y/static/taoge/taoge_list.html");
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

void MusicDownLoadQueryQQPlaylistThread::startToSearch(const QString &playlist)
{
    if(!m_manager)
    {
        return;
    }
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_PLAYLIST_ATTR_URL, false).arg(playlist);

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

void MusicDownLoadQueryQQPlaylistThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        bytes = bytes.replace("MusicJsonCallback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                m_pageTotal = value["sum"].toLongLong();
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicPlaylistItem item;
                    item.m_coverUrl = value["imgurl"].toString();
                    item.m_id = value["dissid"].toString();
                    item.m_name = value["dissname"].toString();
                    item.m_playCount = QString::number(value["listennum"].toULongLong());
                    item.m_description = value["introduction"].toString();
                    item.m_updateTime = value["commit_time"].toString();

                    value = value["creator"].toMap();
                    item.m_nickname = value["name"].toString();

                    emit createPlaylistItems(item);
                }
            }
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryQQPlaylistThread::getDetailsFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(reply && m_manager && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        bytes = bytes.replace("jsonCallback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("cdlist"))
            {
                QVariantList datas = value["cdlist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    QVariantList tags = value["tags"].toList();
                    QString tagsString;
                    foreach(const QVariant &tag, tags)
                    {
                        if(tag.isNull())
                        {
                            continue;
                        }

                        tagsString.append(tag.toMap()["name"].toString() + "|");
                    }
                    m_rawData["tags"] = tagsString;

                    value = var.toMap();
                    QVariantList songLists = value["songlist"].toList();
                    foreach(const QVariant &var, songLists)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        MusicObject::MusicSongInfomation musicInfo;
                        foreach(const QVariant &var, value["singer"].toList())
                        {
                            if(var.isNull())
                            {
                                continue;
                            }
                            QVariantMap name = var.toMap();
                            musicInfo.m_singerName = name["name"].toString();
                            musicInfo.m_artistId = QString::number(name["id"].toULongLong());
                        }
                        musicInfo.m_songName = value["songname"].toString();
                        musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt()*1000);;

                        musicInfo.m_songId = value["songid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();

                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                        musicInfo.m_smallPicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false)
                                    .arg(musicInfo.m_albumId.right(2).left(1))
                                    .arg(musicInfo.m_albumId.right(1)).arg(musicInfo.m_albumId);
                        readFromMusicSongAttribute(&musicInfo, m_manager, value, m_searchQuality, m_queryAllRecords);

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
    }

    emit downLoadDataChanged(QString());
}
