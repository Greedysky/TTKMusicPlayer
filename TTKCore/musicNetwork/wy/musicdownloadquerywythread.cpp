#include "musicdownloadquerywythread.h"
#include "musicnumberdefine.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryWYThread::MusicDownLoadQueryWYThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_index = 0;
}

QString MusicDownLoadQueryWYThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(QUrl(MusicCryptographicHash::decryptData(WY_SEARCH_URL, URL_KEY)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->post(request, MusicCryptographicHash::decryptData(WY_SEARCH_QUERY_URL, URL_KEY).arg(text).arg(0).toUtf8());
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
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    ++m_index;
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation info;
                    info.m_songName = value["name"].toString();
                    info.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                    info.m_songId = QString::number(value["id"].toInt());
                    info.m_lrcUrl = MusicCryptographicHash::decryptData(WY_SONG_LRC_URL, URL_KEY).arg(value["id"].toInt());

                    QVariantMap albumObject = value["album"].toMap();
                    info.m_smallPicUrl = albumObject["picUrl"].toString();
                    info.m_albumId = QString::number(albumObject["id"].toInt());

                    QVariantList artistsArray = value["artists"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        info.m_singerName = artistMap["name"].toString();
                    }

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(&info, value["lMusic"].toMap(), MB_32);
                        readFromMusicSongAttribute(&info, value["bMusic"].toMap(), MB_128);
                        readFromMusicSongAttribute(&info, value["mMusic"].toMap(), MB_192);
                        readFromMusicSongAttribute(&info, value["hMusic"].toMap(), MB_320);
                    }
                    else
                    {
                        if(m_searchQuality == tr("ST"))
                            readFromMusicSongAttribute(&info, value["lMusic"].toMap(), MB_32);
                        if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(&info, value["bMusic"].toMap(), MB_128);
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(&info, value["mMusic"].toMap(), MB_192);
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(&info, value["hMusic"].toMap(), MB_320);
                    }

                    if(info.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    emit createSearchedItems(info.m_songName, info.m_singerName, info.m_timeLength);
                    m_musicSongInfos << info;

                    if( m_index >= m_songIds.count())
                    {
                        emit downLoadDataChanged(QString());
                        deleteAll();
                    }
                }
                if(m_musicSongInfos.count() == 0)
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
                MusicObject::MusicSongInfomation info;
                info.m_songName = value["name"].toString();
                info.m_singerName = value["artistName"].toString();
                info.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

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
                    info.m_songAttrs.append(attr);
                }

                if(info.m_songAttrs.isEmpty())
                {
                    return;
                }

                emit createSearchedItems(info.m_songName, info.m_singerName, info.m_timeLength);
                m_musicSongInfos << info;

                if( m_index >= m_songIds.count() || m_musicSongInfos.count() == 0)
                {
                    emit downLoadDataChanged(QString());
                    deleteAll();
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

void MusicDownLoadQueryWYThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                            const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    qlonglong dfsId = key.value("dfsId").toLongLong();
    attr.m_bitrate = bitrate;
    attr.m_format = key.value("extension").toString();
    attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
    attr.m_url = MusicCryptographicHash::decryptData(WY_SONG_PATH_URL, URL_KEY).arg(encryptedId(dfsId)).arg(dfsId);
    info->m_songAttrs.append(attr);
}

QString MusicDownLoadQueryWYThread::encryptedId(qlonglong id)
{
    return encryptedId(QString::number(id));
}

QString MusicDownLoadQueryWYThread::encryptedId(const QString &string)
{
    QByteArray array1(WY_ENCRYPT_STRING);
    QByteArray array2 = string.toUtf8();
    int length = array1.length();
    for(int i=0; i<array2.length(); ++i)
    {
        array2[i] = array2[i]^array1[i%length];
    }

    QByteArray encodedData = QCryptographicHash::hash(array2, QCryptographicHash::Md5);
#if (defined MUSIC_GREATER_NEW && !defined MUSIC_NO_WINEXTRAS)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return encodedData;
}
