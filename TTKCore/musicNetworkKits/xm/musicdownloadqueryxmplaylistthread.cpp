#include "musicdownloadqueryxmplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMPlaylistThread::MusicDownLoadQueryXMPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_index = 0;
    m_queryServer = "XiaMi";
}

QString MusicDownLoadQueryXMPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMPlaylistThread::startSearchSong(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startSearchSong(playlist);
    }
    else
    {
        startSearchSongAll(playlist);
    }
}

void MusicDownLoadQueryXMPlaylistThread::startSearchSongAll(const QString &type)
{
    QString key = type.isEmpty() ? "108051" : type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_PLAYLIST_URL, URL_KEY).arg(key);
    deleteAll();

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

void MusicDownLoadQueryXMPlaylistThread::startSearchSongAll(const QSet<QString> &ids)
{
    foreach(const QString &id, ids)
    {
        QUrl musicUrl = MusicCryptographicHash::decryptData(XM_PLAYLIST_ATTR_URL, URL_KEY).arg(id);

        QNetworkRequest request;
        request.setUrl(musicUrl);
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    #ifndef QT_NO_SSL
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
    #endif
        QNetworkReply *reply = m_manager->get(request);
        connect(reply, SIGNAL(finished()), SLOT(getSongAllFinished()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
    }
}

void MusicDownLoadQueryXMPlaylistThread::startSearchSong(const QString &playlist)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_PLAYLIST_IATTR_URL, URL_KEY).arg(playlist);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(getDetailsFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMPlaylistThread::downLoadFinished()
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
        QSet<QString> songIds;
        while(m_reply->canReadLine())
        {
            QString text = m_reply->readLine();
            QRegExp regx(QString("/collect/(\\d+)"));
            int pos = QString(text).indexOf(regx);
            while(pos != -1)
            {
                songIds << regx.cap(1);
                pos += regx.matchedLength();
                pos = regx.indexIn(text, pos);
            }
        }
        startSearchSongAll(songIds);
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryXMPlaylistThread::getSongAllFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QString dataSplit = QString(bytes).remove("\r\n").remove("\n").remove("\t");

        MusicPlaylistItem item;
        ///////////////////////////////////////////////////////////////////////////
        QStringList dataSplits = dataSplit.split("<div class=\"info_collect_main\">");
        QStringList frontSplits = dataSplits.front().split("<a class=\"bigImgCover\"");
        dataSplit = frontSplits.front();
        dataSplit = dataSplit.split("<link rel=\"canonical\"").back().split("/>").front();
        item.m_id = dataSplit.remove("\"").split("/").last().trimmed();
        ///////////////////////////////////////////////////////////////////////////
        dataSplit = frontSplits.back();
        item.m_coverUrl = dataSplit.split("href=\"").back().split("\"").front();

        dataSplit = dataSplits.last();
        ///////////////////////////////////////////////////////////////////////////
        item.m_description = dataSplit.split("<div class=\"info_intro_full\" ><br>").back().split("<br").front();

        dataSplits = dataSplit.split("</div>");
        dataSplit = dataSplits.front();
        dataSplits = dataSplit.split("<span></span></h2>");

        item.m_name = dataSplits.front().remove("<h2>");
        ///////////////////////////////////////////////////////////////////////////
        dataSplits = dataSplits.back().split("<li><span>");
        if(dataSplits.count() == 6)
        {
            item.m_nickname = dataSplits[1].remove("</a></li>").split("title=\"\">").last().split("</a>").front();

            QString tags;
            int index = 0;
            foreach(const QString &var, dataSplits[3].split("</a>,<a href="))
            {
                if(++index >= 4)
                {
                    break;
                }
                tags.append(var.split("\">").last() + "|");
            }
            item.m_tags = tags;

            item.m_updateTime = dataSplits[4].split("</span>").last().remove("</li>");
            item.m_playCount = dataSplits[5].remove("&nbsp;").split("<span>").front().split("</span>").last();
        }
        ///////////////////////////////////////////////////////////////////////////
        emit createPlaylistItems(item);
    }

//    emit downLoadDataChanged(QString());
}

void MusicDownLoadQueryXMPlaylistThread::getDetailsFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_songIds.clear();
    m_index = 0;

    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["state"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    m_songIds << QString::number(value["song_id"].toULongLong());
                }

                if(!m_songIds.isEmpty())
                {
                    startSongListQuery();
                }
                else
                {
                    emit downLoadDataChanged(QString());
                }
            }
            else
            {
                emit downLoadDataChanged(QString());
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
        }
    }
    else
    {
        emit downLoadDataChanged(QString());
    }
}

void MusicDownLoadQueryXMPlaylistThread::songListFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                ++m_index;
                value = value["data"].toMap();
                QVariantList datas = value["trackList"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["singers"].toString();
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);

                    musicInfo.m_songId = QString::number(value["songId"].toULongLong());
                    musicInfo.m_artistId = QString::number(value["artistId"].toULongLong());
                    musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(XM_SONG_PIC_URL, URL_KEY) +
                                              value["album_logo"].toString().replace("_1.", "_4.");
                    QString lrcUrl = value["lyric_url"].toString();
                    if(!lrcUrl.endsWith("txt"))
                    {
                        musicInfo.m_lrcUrl = lrcUrl;
                    }

                    ///music normal songs urls
                    QVariantList auditions = value["allAudios"].toList();
                    foreach(const QVariant &audition, auditions)
                    {
                        QVariantMap audUrlsValue = audition.toMap();
                        if(audUrlsValue.isEmpty())
                        {
                            continue;
                        }

                        readFromMusicSongAttribute(&musicInfo, audUrlsValue, m_searchQuality, m_queryAllRecords);
                    }

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

                if(m_index >= m_songIds.count())
                {
                    emit downLoadDataChanged(QString());
                }
            }
            else
            {
                emit downLoadDataChanged(QString());
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
        }
    }
    else
    {
        emit downLoadDataChanged(QString());
    }
}

void MusicDownLoadQueryXMPlaylistThread::startSongListQuery()
{
    foreach(const QString &id, m_songIds)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(MusicCryptographicHash::decryptData(XM_ARTIST_PLAYLIST_URL, URL_KEY).arg(id)));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
        request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    #ifndef QT_NO_SSL
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
    #endif
        QNetworkReply *reply = m_manager->get(request);
        connect(reply, SIGNAL(finished()), SLOT(songListFinished()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
    }
}
