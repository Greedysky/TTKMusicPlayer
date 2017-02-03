#include "musicdownloadquerykwalbumthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWAlbumThread::MusicDownLoadQueryKWAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWAlbumThread::startSearchSong(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startSearchSong(album);
}

void MusicDownLoadQueryKWAlbumThread::startSearchSong(const QString &album)
{
    m_searchText = album;
    QUrl musicUrl = MusicCryptographicHash::decryptData(KW_ALBUM_URL, URL_KEY).arg(album);

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
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWAlbumThread::downLoadFinished()
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

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes.replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(!value.isEmpty() && value.contains("musiclist"))
            {
                QString albumId = value["name"].toString() + "<>" +
                                  value["lang"].toString() + "<>" +
                                  value["company"].toString() + "<>" +
                                  value["pub"].toString();

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
                    musicInfo.m_timeLength = "-";

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["id"].toString();
                        musicInfo.m_artistId = value["artistid"].toString();
                        musicInfo.m_albumId = albumId;

                        readFromMusicSongPic(&musicInfo, musicInfo.m_songId, m_manager);
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(KW_SONG_INFO_URL, URL_KEY).arg(musicInfo.m_songId);

                        ///music normal songs urls
                        QString auditions = value["formats"].toString();
                        qDebug() << auditions;
                        if(m_queryAllRecords)
                        {
                            readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_128);
                            readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_192);
                            readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_320);
                            readFromMusicSongAttribute(&musicInfo, "ape", auditions, musicInfo.m_songId, MB_1000);
                        }
                        else
                        {
                            if(m_searchQuality == tr("SD"))
                                readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_128);
                            else if(m_searchQuality == tr("HD"))
                                readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_192);
                            else if(m_searchQuality == tr("SQ"))
                                readFromMusicSongAttribute(&musicInfo, "mp3", auditions, musicInfo.m_songId, MB_320);
                            else if(m_searchQuality == tr("CD"))
                                readFromMusicSongAttribute(&musicInfo, "ape", auditions, musicInfo.m_songId, MB_1000);
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
