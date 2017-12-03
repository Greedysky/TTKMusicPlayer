#include "musicdownloadquerymgartistthread.h"
#include "musicnumberutils.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryMGArtistThread::MusicDownLoadQueryMGArtistThread(QObject *parent)
    : MusicDownLoadQueryArtistThread(parent)
{
    m_queryServer = "Migu";
}

QString MusicDownLoadQueryMGArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryMGArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(MG_ARTIST_URL, false).arg(artist);
    deleteAll();
    m_interrupt = true;
    m_searchText = artist;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryMGArtistThread::downLoadFinished()
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
            if(value["code"].toString() == "000000" && value.contains("songs"))
            {
                bool artistFlag = false;
                ////////////////////////////////////////////////////////////
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = value["singer"].toString();
                    musicInfo.m_songName = value["title"].toString();
                    musicInfo.m_timeLength = "-";

                    musicInfo.m_songId = value["contentid"].toString();
                    musicInfo.m_artistId = value["singerid"].toString();
                    musicInfo.m_albumId = value["albumid"].toString();

                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(musicInfo.m_songId);
                    musicInfo.m_smallPicUrl = value["albumImg"].toString();
                    musicInfo.m_albumName = value["album"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, true);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    ////////////////////////////////////////////////////////////
                    for(int i=0; i<musicInfo.m_songAttrs.count(); ++i)
                    {
                        MusicObject::MusicSongAttribute *attr = &musicInfo.m_songAttrs[i];
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        if(attr->m_size.isEmpty())
                        {
                            attr->m_size = MusicUtils::Number::size2Label(getUrlFileSize(attr->m_url));
                        }
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    }
                    ////////////////////////////////////////////////////////////
                    if(!artistFlag)
                    {
                        artistFlag = true;
                        MusicPlaylistItem info;
                        info.m_description = value["detail"].toString();
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
