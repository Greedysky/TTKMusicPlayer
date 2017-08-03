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

void MusicDownLoadQueryKWArtistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}

void MusicDownLoadQueryKWArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_ARTIST_URL, false).arg(artist).arg(0).arg(50);
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

void MusicDownLoadQueryKWArtistThread::downLoadFinished()
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

                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongPic(&musicInfo, musicInfo.m_songId);
                        if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(KW_SONG_INFO_URL, false).arg(musicInfo.m_songId);
                        ///music normal songs urls
                        readFromMusicSongAttribute(&musicInfo, value["FORMATS"].toString(), m_searchQuality, m_queryAllRecords);
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
