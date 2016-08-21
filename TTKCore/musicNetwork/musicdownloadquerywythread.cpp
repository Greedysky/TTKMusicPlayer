#include "musicdownloadquerywythread.h"
#include "musicnumberdefine.h"
#include "musicutils.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   ///QJson import
#   include "qjson/parser.h"
#endif
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
    request.setUrl(QUrl("http://music.163.com/api/search/get"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", "http://music.163.com");
    request.setRawHeader("Referer", "http://music.163.com");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->post(request, QString("s=%1&type=1&limit=40&offset=0").arg(text).toUtf8());
    connect(reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYThread::downLoadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        QByteArray bytes = reply->readAll();
        m_musicSongInfos.clear();
        m_index = 0;
#ifdef MUSIC_GREATER_NEW
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError ||
            !parseDoucment.isObject())
        {
            return;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.contains("code") && jsonObject.value("code").toInt() == 200)
        {
            jsonObject = jsonObject.value("result").toObject();
            if(jsonObject.contains("songs"))
            {
                QJsonArray songsArray = jsonObject.take("songs").toArray();
                foreach(QJsonValue value, songsArray)
                {
                    if(!value.isObject())
                    {
                       continue;
                    }
                    QJsonObject object = value.toObject();
                    MusicObject::MusicSongInfomation infomation;
                    if(m_currentType != MovieQuery)
                    {
                        infomation.m_id = QString::number(object.value("id").toVariant().toLongLong());
                    }
                    else
                    {
                        infomation.m_id = QString::number(object.value("mvid").toVariant().toLongLong());
                    }
                    if(infomation.m_id != QString("0"))
                    {
                        m_musicSongInfos << infomation;
                    }
                }
            }
        }
#else
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
                    MusicObject::MusicSongInfomation infomation;
                    if(m_currentType != MovieQuery)
                    {
                        infomation.m_id = QString::number(value["id"].toLongLong());
                    }
                    else
                    {
                        infomation.m_id = QString::number(value["mvid"].toLongLong());
                    }
                    if(infomation.m_id != QString("0"))
                    {
                        m_musicSongInfos << infomation;
                    }
                }
            }
        }
#endif
        (m_currentType != MovieQuery) ? startSongListQuery() : startMVListQuery();
    }
}

void MusicDownLoadQueryWYThread::songListFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        QByteArray bytes = reply->readAll();
#ifdef MUSIC_GREATER_NEW
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError ||
            !parseDoucment.isObject())
        {
            emit downLoadDataChanged(QString());
            return;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.contains("code") && jsonObject.value("code").toInt() == 200 &&
           jsonObject.contains("songs"))
        {
            QJsonArray songsArray = jsonObject.take("songs").toArray();
            foreach(QJsonValue songsValue, songsArray)
            {
                if(!songsValue.isObject())
                {
                   continue;
                }

                QJsonObject object = songsValue.toObject();
                int index = foundMusicInfoById(object.value("id").toInt());
                if(index != -1)
                {
                    MusicObject::MusicSongInfomation *info = &m_musicSongInfos[index];
                    info->m_songName = object.value("name").toString();
                    info->m_timeLength = QString::number(object.value("duration").toInt());
                    info->m_lrcUrl = "http://music.163.com/api/song/lyric?lv=-1&id=" + QString::number(object.value("id").toInt());

                    QJsonArray artistsArray = object.value("artists").toArray();
                    foreach(QJsonValue artistValue, artistsArray)
                    {
                        if(!artistValue.isObject())
                        {
                           continue;
                        }

                        QJsonObject artistObject = artistValue.toObject();
                        info->m_singerName = artistObject.value("name").toString();
                        info->m_smallPicUrl = artistObject.value("picUrl").toString();
                    }

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(info, object.value("lMusic").toVariant().toMap(), MB_32);
                        readFromMusicSongAttribute(info, object.value("bMusic").toVariant().toMap(), MB_128);
                        readFromMusicSongAttribute(info, object.value("mMusic").toVariant().toMap(), MB_192);
                        readFromMusicSongAttribute(info, object.value("hMusic").toVariant().toMap(), MB_320);
                    }
                    else
                    {
                        if(m_searchQuality == tr("ST"))
                            readFromMusicSongAttribute(info, object.value("lMusic").toVariant().toMap(), MB_32);
                        if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(info, object.value("bMusic").toVariant().toMap(), MB_128);
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(info, object.value("mMusic").toVariant().toMap(), MB_192);
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(info, object.value("hMusic").toVariant().toMap(), MB_320);
                    }

                    if(info->m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    emit createSearchedItems(info->m_songName, info->m_singerName, info->m_timeLength);

                    if( ++m_index >= m_musicSongInfos.count())
                    {
                        emit downLoadDataChanged(QString());
                    }
                }
            }
        }
#else
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
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    int index = foundMusicInfoById(value["id"].toInt());
                    if(index != -1)
                    {
                        MusicObject::MusicSongInfomation *info = &m_musicSongInfos[index];
                        info->m_songName = value["name"].toString();
                        info->m_timeLength = QString::number(value["duration"].toInt());
                        info->m_lrcUrl = "http://music.163.com/api/song/lyric?lv=-1&id=" + QString::number(value["id"].toInt());

                        QVariantList artistsArray = value["artists"].toList();
                        foreach(QVariant artistValue, artistsArray)
                        {
                            if(artistValue.isNull())
                            {
                                continue;
                            }

                            QVariantMap artistMap = artistValue.toMap();
                            info->m_singerName = artistMap["name"].toString();
                            info->m_smallPicUrl = artistMap["picUrl"].toString();
                        }

                        if(m_queryAllRecords)
                        {
                            readFromMusicSongAttribute(info, value["lMusic"].toMap(), MB_32);
                            readFromMusicSongAttribute(info, value["bMusic"].toMap(), MB_128);
                            readFromMusicSongAttribute(info, value["mMusic"].toMap(), MB_192);
                            readFromMusicSongAttribute(info, value["hMusic"].toMap(), MB_320);
                        }
                        else
                        {
                            if(m_searchQuality == tr("ST"))
                                readFromMusicSongAttribute(info, value["lMusic"].toMap(), MB_32);
                            if(m_searchQuality == tr("SD"))
                                readFromMusicSongAttribute(info, value["bMusic"].toMap(), MB_128);
                            else if(m_searchQuality == tr("HD"))
                                readFromMusicSongAttribute(info, value["mMusic"].toMap(), MB_192);
                            else if(m_searchQuality == tr("SQ"))
                                readFromMusicSongAttribute(info, value["hMusic"].toMap(), MB_320);
                        }

                        if(info->m_songAttrs.isEmpty())
                        {
                            continue;
                        }
                        emit createSearchedItems(info->m_songName, info->m_singerName, info->m_timeLength);

                        if( ++m_index >= m_musicSongInfos.count())
                        {
                            emit downLoadDataChanged(QString());
                        }
                    }
                }
            }
        }
#endif
    }
}

void MusicDownLoadQueryWYThread::mvListFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(reply)
    {
        QByteArray bytes = reply->readAll();
#ifdef MUSIC_GREATER_NEW
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError ||
            !parseDoucment.isObject())
        {
            emit downLoadDataChanged(QString());
            return;
        }

        QJsonObject object = parseDoucment.object();
        if(object.contains("code") && object.value("code").toInt() == 200)
        {
            object = object.take("data").toObject();
            int index = foundMusicInfoById(object.value("id").toInt());
            if(index != -1)
            {
                MusicObject::MusicSongInfomation *info = &m_musicSongInfos[index];
                info->m_songName = object.value("name").toString();
                info->m_singerName = object.value("artistName").toString();
                info->m_timeLength = QString::number(object.value("duration").toInt());

                QJsonObject brsObject = object.take("brs").toObject();
                foreach(QString key, brsObject.keys())
                {
                    MusicObject::MusicSongAttribute attr;
                    attr.m_bitrate = key.toInt();
                    attr.m_url = brsObject.value(key).toString();
                    attr.m_format = attr.m_url.right(3);
                    attr.m_size = QString();
                    info->m_songAttrs.append(attr);
                }

                if(info->m_songAttrs.isEmpty())
                {
                    return;
                }
                emit createSearchedItems(info->m_songName, info->m_singerName, info->m_timeLength);

                if( ++m_index >= m_musicSongInfos.count())
                {
                    emit downLoadDataChanged(QString());
                }
            }
        }
#else
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["data"].toMap();
                int index = foundMusicInfoById(value["id"].toInt());
                if(index != -1)
                {
                    MusicObject::MusicSongInfomation *info = &m_musicSongInfos[index];
                    info->m_songName = value["name"].toString();
                    info->m_singerName = value["artistName"].toString();
                    info->m_timeLength = QString::number(value["duration"].toInt());

                    value = value["brs"].toMap();
                    foreach(QString key, value.keys())
                    {
                        MusicObject::MusicSongAttribute attr;
                        attr.m_bitrate = key.toInt();
                        attr.m_url = value[key].toString();
                        attr.m_format = attr.m_url.right(3);
                        attr.m_size = QString();
                        info->m_songAttrs.append(attr);
                    }

                    if(info->m_songAttrs.isEmpty())
                    {
                        return;
                    }
                    emit createSearchedItems(info->m_songName, info->m_singerName, info->m_timeLength);

                    if( ++m_index >= m_musicSongInfos.count())
                    {
                        emit downLoadDataChanged(QString());
                    }
                }
            }
        }
#endif
    }
}

void MusicDownLoadQueryWYThread::startSongListQuery()
{
    foreach(MusicObject::MusicSongInfomation indfo, m_musicSongInfos)
    {
        QNetworkRequest request;
        request.setUrl(QUrl("http://music.163.com/api/song/detail?ids=%5B" + indfo.m_id + "%5d"));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Origin", "http://music.163.com");
        request.setRawHeader("Referer", "http://music.163.com");
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
    foreach(MusicObject::MusicSongInfomation indfo, m_musicSongInfos)
    {
        QNetworkRequest request;
        request.setUrl(QUrl("http://music.163.com/api/mv/detail?id=" + indfo.m_id  + "&type=mp4"));
        request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        request.setRawHeader("Origin", "http://music.163.com");
        request.setRawHeader("Referer", "http://music.163.com");
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
    QString dfsId = QString::number(key.value("dfsId").toLongLong());
    attr.m_bitrate = bitrate;
    attr.m_format = key.value("extension").toString();
    attr.m_size = MusicUtils::UNumber::size2Label(key.value("size").toInt());
    attr.m_url = QString("http://m2.music.126.net/%1/%2.mp3").arg(encryptedId(dfsId)).arg(dfsId);
    info->m_songAttrs.append(attr);
}

QString MusicDownLoadQueryWYThread::encryptedId(const QString &string)
{
    QByteArray array1("3go8&$8*3*3h0k(2)2");
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

int MusicDownLoadQueryWYThread::foundMusicInfoById(int id)
{
    for(int i=0; i<m_musicSongInfos.count(); ++i)
    {
        if(m_musicSongInfos[i].m_id.toInt() == id)
        {
            return i;
        }
    }
    return -1;
}
