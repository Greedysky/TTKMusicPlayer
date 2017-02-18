#include "musicdownloadqueryqqalbumthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQAlbumThread::MusicDownLoadQueryQQAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "QQ";
}

QString MusicDownLoadQueryQQAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQAlbumThread::startSearchSong(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startSearchSong(album);
}

void MusicDownLoadQueryQQAlbumThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_ALBUM_URL, URL_KEY).arg(album);

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

void MusicDownLoadQueryQQAlbumThread::downLoadFinished()
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
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QString albumId = "<>" +
                                  value["lan"].toString() + "<>" +
                                  value["company_new"].toMap()["name"].toString() + "<>" +
                                  value["aDate"].toString();
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
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(QQ_SONG_LRC_URL, URL_KEY).arg(musicInfo.m_songId);
                        musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(QQ_SONG_PIC_URL, URL_KEY)
                                    .arg(musicInfo.m_albumId.right(2).left(1))
                                    .arg(musicInfo.m_albumId.right(1)).arg(musicInfo.m_albumId);
                        musicInfo.m_albumId = value["albumname"].toString() + albumId;
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
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
