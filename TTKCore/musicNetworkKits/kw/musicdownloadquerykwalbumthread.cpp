#include "musicdownloadquerykwalbumthread.h"
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

void MusicDownLoadQueryKWAlbumThread::startToSearch(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startToSearch(album);
}

void MusicDownLoadQueryKWAlbumThread::startToSearch(const QString &album)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(album));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_ALBUM_URL, false).arg(album);
    deleteAll();

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
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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

                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongPic(&musicInfo, musicInfo.m_songId);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(musicInfo.m_songId);
                        ///music normal songs urls
                        readFromMusicSongAttribute(&musicInfo, value["formats"].toString(), m_searchQuality, m_queryAllRecords);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

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
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
