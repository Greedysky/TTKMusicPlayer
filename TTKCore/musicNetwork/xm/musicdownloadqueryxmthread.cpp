#include "musicdownloadqueryxmthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMThread::MusicDownLoadQueryXMThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "XiaMi";
}

QString MusicDownLoadQueryXMThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_SONG_SEARCH_URL, URL_KEY).arg(text);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

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
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMThread::downLoadFinished()
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

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                QVariantList datas = value["data"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(value["song_status"].toInt() != 0)
                    {
                        continue;
                    }

                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["singer"].toString();
                    musicInfo.m_songName = value["song_name"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = QString::number(value["song_id"].toULongLong());
                        musicInfo.m_artistId = QString::number(value["artist_id"].toULongLong());
                        musicInfo.m_albumId = QString::number(value["album_id"].toULongLong());
                        musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(XM_SONG_PIC_URL, URL_KEY) +
                                                  value["album_logo"].toString().replace("_1.", "_4.");

                        QVariantMap lrcValue = value["lyric"].toMap();
                        QString lrcUrl = lrcValue["lyricFile"].toString();
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

                            int bitrate = map2NormalBitrate(audUrlsValue["rate"].toInt());
                            if(m_queryAllRecords)
                            {
                                readFromMusicSongAttribute(&musicInfo, audUrlsValue, bitrate);
                            }
                            else
                            {
                                if(m_searchQuality == tr("ST") && bitrate == MB_32)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_32);
                                else if(m_searchQuality == tr("SD") && bitrate == MB_128)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_128);
                                else if(m_searchQuality == tr("HD") && bitrate == MB_192)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_192);
                                else if(m_searchQuality == tr("SQ") && bitrate == MB_320)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_320);
                                else if(m_searchQuality == tr("CD") && bitrate == MB_500)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_500);
                            }
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
//                        value["mvUrl"].toString();

//                        if(musicInfo.m_songAttrs.isEmpty())
//                        {
//                            continue;
//                        }

//                        emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
//                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
