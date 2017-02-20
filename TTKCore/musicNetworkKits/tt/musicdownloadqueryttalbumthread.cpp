#include "musicdownloadqueryttalbumthread.h"
#include "musicdownloadttinterface.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryTTAlbumThread::MusicDownLoadQueryTTAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "TTpod";
}

QString MusicDownLoadQueryTTAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryTTAlbumThread::startSearchSong(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startSearchSong(album);
}

void MusicDownLoadQueryTTAlbumThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(TT_ALBUM_URL, URL_KEY).arg(album);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryTTAlbumThread::downLoadFinished()
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
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 1 || value["code"].toInt() == 200)
            {
                QVariantList datas = value["data"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;

                    QString songId = QString::number(value["songId"].toULongLong());
                    QString songName = value["name"].toString();
                    QString singerName = value["singerName"].toString();

                    if(m_currentType != MovieQuery)
                    {
                        readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        QString duration = musicInfo.m_songAttrs.first().m_duration;
                        MusicSearchedItem item;
                        item.m_songname = songName;
                        item.m_artistname = singerName;
                        item.m_time = duration;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItems(item);

                        musicInfo.m_albumId = value["albumName"].toString() + "<>" +
                                              QString::number(value["lang"].toInt()) + "<>" +
                                              value["company"].toString() + "<>" +
                                              QString::number(value["releaseYear"].toInt());
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(TT_SONG_LRC_URL, URL_KEY).arg(singerName).arg(songName).arg(songId);
                        musicInfo.m_smallPicUrl = value["picUrl"].toString();
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        musicInfo.m_timeLength = duration;
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
