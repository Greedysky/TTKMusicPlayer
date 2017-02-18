#include "musicdownloadqueryqqthread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQThread::MusicDownLoadQueryQQThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "QQ";
}

QString MusicDownLoadQueryQQThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_SONG_SEARCH_URL, URL_KEY).arg(text).arg(0).arg(50);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryQQThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
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

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["songid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();
                        if(!m_querySimplify)
                        {
                            musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(QQ_SONG_LRC_URL, URL_KEY).arg(musicInfo.m_songId);
                            musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(QQ_SONG_PIC_URL, URL_KEY)
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
                        }
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        //MV
                        QString mvId = value["vid"].toString();
                        readFromMusicMVAttribute(&musicInfo, mvId);

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

void MusicDownLoadQueryQQThread::readFromMusicMVAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QString &id)
{
    if(id.isEmpty())
    {
        return;
    }

    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_MV_INFO_URL, URL_KEY).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bytes = reply->readAll();
    bytes.replace("QZOutputJson=", "");
    bytes.chop(1);

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("fl"))
        {
            QString urlPrefix;
            QVariantMap vlValue = value["vl"].toMap();
            QVariantList viLists = vlValue["vi"].toList();
            if(!viLists.isEmpty())
            {
                vlValue = viLists.first().toMap();
                vlValue = vlValue["ul"].toMap();
                viLists = vlValue["ui"].toList();
                vlValue = viLists.first().toMap();
                urlPrefix = vlValue["url"].toString();
            }

            QVariantMap flValue = value["fl"].toMap();
            QVariantList mvLists = flValue["fi"].toList();
            foreach(const QVariant &var, mvLists)
            {
                if(var.isNull())
                {
                    continue;
                }

                flValue = var.toMap();
                MusicObject::MusicSongAttribute attr;
                attr.m_size = MusicUtils::Number::size2Label(flValue["fs"].toInt());
                attr.m_format = "mp4";

                int bitRate = flValue["br"].toInt();
                if(bitRate > 375 && bitRate <= 625)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 625 && bitRate <= 875)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 875)
                    attr.m_bitrate = MB_1000;
                else
                    attr.m_bitrate = bitRate;

                bitRate = flValue["id"].toULongLong();
                QString key = getMovieKey(bitRate, id);
                if(!key.isEmpty())
                {
                    QString fn = QString("%1.p%2.1.mp4").arg(id).arg(bitRate - 10000);
                    attr.m_url = QString("%1%2?vkey=%3").arg(urlPrefix).arg(fn).arg(key);
                    info->m_songAttrs.append(attr);
                }
            }
        }
    }
}

QString MusicDownLoadQueryQQThread::getMovieKey(int id, const QString &videoId)
{
    if(videoId.isEmpty())
    {
        return QString();
    }

    QString fn = QString("%1.p%2.1.mp4").arg(videoId).arg(id - 10000);
    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_MV_KEY_URL, URL_KEY).arg(id).arg(videoId).arg(fn);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bytes = reply->readAll();
    bytes.replace("QZOutputJson=", "");
    bytes.chop(1);

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("key"))
        {
            return value["key"].toString();
        }
    }

    return QString();
}
