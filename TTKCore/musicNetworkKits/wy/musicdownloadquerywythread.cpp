#include "musicdownloadquerywythread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYThread::MusicDownLoadQueryWYThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_index = 0;
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_SEARCH_URL, URL_KEY);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->post(request, MusicCryptographicHash::decryptData(WY_SONG_QUERY_URL, URL_KEY).arg(text).arg(0).toUtf8());
    connect(reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYThread::downLoadFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        emit clearAllItems();
        m_musicSongInfos.clear();
        m_songIds.clear();
        m_index = 0;

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["result"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(m_currentType != MovieQuery)
                    {
                        m_songIds << QString::number(value["id"].toLongLong());
                    }
                    else
                    {
                        m_songIds << QString::number(value["mvid"].toLongLong());
                    }
                    m_songIds.remove("0");
                }
            }
        }
        (m_currentType != MovieQuery) ? startSongListQuery() : startMVListQuery();
    }
    else
    {
        emit downLoadDataChanged(QString());
        deleteAll();
    }
}

void MusicDownLoadQueryWYThread::songListFinished()
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
            if(value.contains("code") && value.contains("songs") && value["code"].toInt() == 200)
            {
                ++m_index;
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_songName = value["name"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                    musicInfo.m_songId = QString::number(value["id"].toInt());
                    musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(WY_SONG_LRC_URL, URL_KEY).arg(value["id"].toInt());

                    QVariantMap albumObject = value["album"].toMap();
                    musicInfo.m_smallPicUrl = albumObject["picUrl"].toString();
                    musicInfo.m_albumId = QString::number(albumObject["id"].toInt());

                    QVariantList artistsArray = value["artists"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        musicInfo.m_artistId = QString::number(artistMap["id"].toULongLong());
                        musicInfo.m_singerName = artistMap["name"].toString();
                    }

                    if(!m_querySimplify)
                    {
                        readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);

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
                    }
                    m_musicSongInfos << musicInfo;
                }

                if( m_index >= m_songIds.count())
                {
                    emit downLoadDataChanged(QString());
                    deleteAll();
                    return;
                }
            }
            else
            {
                qint64 albumId = foundAblumIdBySongId(m_songIds.toList()[m_index]);
                ++m_index;

                if(albumId != -1)
                {
                    foundLostSongs(QString::number(albumId));
                }
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
            deleteAll();
        }
    }
    else
    {
        emit downLoadDataChanged(QString());
        deleteAll();
    }
}

void MusicDownLoadQueryWYThread::mvListFinished()
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
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                ++m_index;
                value = value["data"].toMap();
                MusicObject::MusicSongInfomation musicInfo;
                musicInfo.m_songName = value["name"].toString();
                musicInfo.m_singerName = value["artistName"].toString();
                musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

                value = value["brs"].toMap();
                foreach(const QString &key, value.keys())
                {
                    int bit = key.toInt();
                    MusicObject::MusicSongAttribute attr;
                    if(bit > 375 && bit <= 625)
                        attr.m_bitrate = MB_500;
                    else if(bit > 625 && bit <= 875)
                        attr.m_bitrate = MB_750;
                    else if(bit > 875)
                        attr.m_bitrate = MB_1000;
                    else
                        attr.m_bitrate = bit;

                    attr.m_url = value[key].toString();
                    attr.m_format = attr.m_url.right(3);
                    attr.m_size = QString();
                    musicInfo.m_songAttrs.append(attr);
                }

                if(musicInfo.m_songAttrs.isEmpty())
                {
                    return;
                }

                MusicSearchedItem item;
                item.m_songname = musicInfo.m_songName;
                item.m_artistname = musicInfo.m_singerName;
                item.m_time = musicInfo.m_timeLength;
                item.m_type = mapQueryServerString();
                emit createSearchedItems(item);
                m_musicSongInfos << musicInfo;

                if( m_index >= m_songIds.count() )
                {
                    foundOtherMovie();
                }
            }
            else
            {
                foundOtherMovie();
            }
        }
        else
        {
            foundOtherMovie();
        }
    }
    else
    {
        foundOtherMovie();
    }
}

void MusicDownLoadQueryWYThread::startSongListQuery()
{
    foreach(const QString &id, m_songIds)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(MusicCryptographicHash::decryptData(WY_SONG_URL, URL_KEY).arg(id)));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
        request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    #ifndef QT_NO_SSL
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
    #endif
        QNetworkReply *reply = m_manager->get(request);
        connect(reply, SIGNAL(finished()), SLOT(songListFinished()) );
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
    }
}

void MusicDownLoadQueryWYThread::startMVListQuery()
{
    foreach(const QString &id, m_songIds)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(MusicCryptographicHash::decryptData(WY_SONG_MV_URL, URL_KEY).arg(id)));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
        request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    #ifndef QT_NO_SSL
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
    #endif
        QNetworkReply *reply = m_manager->get(request);
        connect(reply, SIGNAL(finished()), SLOT(mvListFinished()) );
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
    }
}

qint64 MusicDownLoadQueryWYThread::foundAblumIdBySongId(const QString &song)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_DETAIL_URL, URL_KEY);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->post( request, QString("c=[{\"id\":\"%1\"}]").arg(song).toUtf8());
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("songs") && value.contains("code") && value["code"].toInt() == 200)
        {
            QVariantList datas = value["songs"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                value = value["al"].toMap();
                return value.contains("id") ? value["id"].toULongLong() : -1;
            }
        }
    }

    return -1;
}

void MusicDownLoadQueryWYThread::foundLostSongs(const QString &ablum)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_ALBUM_URL, URL_KEY).arg(ablum);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get( request );
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("songs"))
        {
            QVariantList datas = value["songs"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicObject::MusicSongInfomation musicInfo;
                musicInfo.m_songName = value["name"].toString();
                musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                musicInfo.m_songId = QString::number(value["id"].toInt());
                musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(WY_SONG_LRC_URL, URL_KEY).arg(value["id"].toInt());

                QVariantMap albumObject = value["album"].toMap();
                musicInfo.m_smallPicUrl = albumObject["picUrl"].toString();
                musicInfo.m_albumId = QString::number(albumObject["id"].toInt());

                QVariantList artistsArray = albumObject["artists"].toList();
                foreach(const QVariant &artistValue, artistsArray)
                {
                    if(artistValue.isNull())
                    {
                        continue;
                    }
                    QVariantMap artistMap = artistValue.toMap();
                    musicInfo.m_artistId = QString::number(artistMap["id"].toULongLong());
                    musicInfo.m_singerName = artistMap["name"].toString();
                }

                readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);

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

                if( m_index >= m_songIds.count())
                {
                    emit downLoadDataChanged(QString());
                    deleteAll();
                    return;
                }
            }
        }
    }
}

void MusicDownLoadQueryWYThread::foundOtherMovie()
{
    ///extra yyt movie
    if(m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *yyt = new MusicDownLoadQueryYYTThread(this);
        connect(yyt, SIGNAL(createSearchedItems(MusicSearchedItem)), SIGNAL(createSearchedItems(MusicSearchedItem)));
        connect(yyt, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        yyt->startSearchSong(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << yyt->getMusicSongInfos();
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
