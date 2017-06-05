#include "musicdownloadquerykwthread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWThread::MusicDownLoadQueryKWThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWThread::startSearchSong(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(KW_SONG_SEARCH_URL, URL_KEY).arg(text).arg(0).arg(50);
    deleteAll();

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

void MusicDownLoadQueryKWThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes.replace("'", "\""), &ok);

        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                QVariantList datas = value["abslist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["ARTIST"].toString();
                    musicInfo.m_songName = value["SONGNAME"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["DURATION"].toString().toInt()*1000);

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                        musicInfo.m_artistId = value["ARTISTID"].toString();
                        musicInfo.m_albumId = value["ALBUMID"].toString();
                        if(!m_querySimplify)
                        {
                            readFromMusicSongPic(&musicInfo, musicInfo.m_songId, m_manager);
                            musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(KW_SONG_INFO_URL, URL_KEY).arg(musicInfo.m_songId);
                            ///music normal songs urls
                            readFromMusicSongAttribute(&musicInfo, value["FORMATS"].toString(), m_searchQuality, m_queryAllRecords);

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
                        //mv
                        musicInfo.m_songId = value["MUSICRID"].toString().replace("MUSIC_", "");
                        readFromMusicMVInfoAttribute(&musicInfo, MB_750, musicInfo.m_songId, "mp4");
                        readFromMusicMVInfoAttribute(&musicInfo, MB_1000, musicInfo.m_songId, "mkv");

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
    if(m_queryExtraMovie && m_currentType == MovieQuery)
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

void MusicDownLoadQueryKWThread::readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info, int bitrate,
                                                              const QString &id, const QString &format)
{
    if(id.isEmpty())
    {
        return;
    }

    MusicObject::MusicSongAttribute attr;
    attr.m_bitrate = bitrate;
    attr.m_format = format;
    attr.m_size = "-";
    attr.m_url = MusicCryptographicHash::decryptData(KW_MV_ATTR_URL, URL_KEY).arg(id).arg(format);
    info->m_songAttrs.append(attr);
}
