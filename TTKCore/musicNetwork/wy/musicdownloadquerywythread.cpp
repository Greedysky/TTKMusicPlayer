#include "musicdownloadquerywythread.h"
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

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

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
    connect(reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
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

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(&musicInfo, value["lMusic"].toMap(), MB_32);
                        readFromMusicSongAttribute(&musicInfo, value["bMusic"].toMap(), MB_128);
                        readFromMusicSongAttribute(&musicInfo, value["mMusic"].toMap(), MB_192);
                        readFromMusicSongAttribute(&musicInfo, value["hMusic"].toMap(), MB_320);
                    }
                    else
                    {
                        if(m_searchQuality == tr("ST"))
                            readFromMusicSongAttribute(&musicInfo, value["lMusic"].toMap(), MB_32);
                        else if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(&musicInfo, value["bMusic"].toMap(), MB_128);
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(&musicInfo, value["mMusic"].toMap(), MB_192);
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(&musicInfo, value["hMusic"].toMap(), MB_320);
                    }

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                    m_musicSongInfos << musicInfo;

                    if( m_index >= m_songIds.count() || m_musicSongInfos.count() == 0)
                    {
                        emit downLoadDataChanged(QString());
                        deleteAll();
                        return;
                    }
                }
            }
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

                emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                m_musicSongInfos << musicInfo;

                if( m_index >= m_songIds.count() || m_musicSongInfos.count() == 0)
                {
                    emit downLoadDataChanged(QString());
                    deleteAll();
                    return;
                }
            }
        }
    }
    else
    {
        emit downLoadDataChanged(QString());
        deleteAll();
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
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
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
