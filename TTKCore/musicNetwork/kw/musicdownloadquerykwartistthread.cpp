#include "musicdownloadquerykwartistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWArtistThread::MusicDownLoadQueryKWArtistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWArtistThread::startSearchSong(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startSearchSong(artist);
}

void MusicDownLoadQueryKWArtistThread::startSearchSong(const QString &artist)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(KW_ARTIST_URL, URL_KEY).arg(artist).arg(0).arg(50);

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

void MusicDownLoadQueryKWArtistThread::downLoadFinished()
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

                        readFromMusicSongPic(&musicInfo, musicInfo.m_songId, m_manager);
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(KW_SONG_INFO_URL, URL_KEY).arg(musicInfo.m_songId);

                        ///music normal songs urls
                        QString auditions = value["FORMATS"].toString();
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
