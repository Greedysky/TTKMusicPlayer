#include "musicdownloadquerykgartistthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGArtistThread::MusicDownLoadQueryKGArtistThread(QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = "Kugou";
}

QString MusicDownLoadQueryKGArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKGArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_ARTIST_URL, false).arg(artist).arg(0).arg(50);
    deleteAll();
    m_searchText = artist;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKGArtistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

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
                bool artistFlag = false;
                ////////////////////////////////////////////////////////////
                value = value["data"].toMap();
                QVariantList datas = value["info"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["filename"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    if(musicInfo.m_songName.contains("-"))
                    {
                        QStringList ll = musicInfo.m_songName.split("-");
                        musicInfo.m_singerName = ll.front().trimmed();
                        musicInfo.m_songName = ll.back().trimmed();
                    }

                    musicInfo.m_songId = value["hash"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    MusicPlaylistItem albumInfo;
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAlbumInfo(&albumInfo, musicInfo.m_albumId);
                    musicInfo.m_albumName = albumInfo.m_nickName;
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    readFromMusicSongLrcAndPic(&musicInfo);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    ////////////////////////////////////////////////////////////
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicPlaylistItem info;
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        getDownLoadIntro(&info);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        info.m_id = m_searchText;
                        info.m_name = musicInfo.m_singerName;
                        info.m_coverUrl = musicInfo.m_smallPicUrl;
                        emit createArtistInfoItem(info);
                    }
                    ////////////////////////////////////////////////////////////
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKGArtistThread::getDownLoadIntro(MusicPlaylistItem *item)
{
    if(!m_manager)
    {
        return;
    }

    QNetworkRequest request;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_ARTIST_INFO_URL, false).arg(m_searchText);

    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["errcode"].toInt() == 0)
        {
            value = value["data"].toMap();
            item->m_description = value["intro"].toString();
        }
    }
}
