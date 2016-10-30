#include "musicdownloadqueryartistthread.h"
#include "musicsettingmanager.h"
#include "musicnumberdefine.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryArtistThread::MusicDownLoadQueryArtistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryArtistThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    startSearchSong(text);
}

void MusicDownLoadQueryArtistThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = QString(getCurrentURL()).arg(album);
    ///This is a multiple music API

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

void MusicDownLoadQueryArtistThread::downLoadFinished()
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
            QVariantList datas = data.toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                MusicObject::MusicSongInfomation musicInfo;
                QString songName = value["SongName"].toString();
                QString singerName = value["Artist"].toString();
                QString duration = value["Length"].toString();
                QString size = value["Size"].toString();

                readFromMusicSongAttribute(musicInfo, size, MB_1000, value["FlacUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_1000, value["AacUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_320, value["SqUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_192, value["HqUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_128, value["LqUrl"].toString());

                if(musicInfo.m_songAttrs.isEmpty())
                {
                    continue;
                }
                emit createSearchedItems(songName, singerName, duration);

                musicInfo.m_albumId = value["Album"].toString();
                musicInfo.m_songName = songName;
                musicInfo.m_singerName = singerName;
                musicInfo.m_timeLength = duration;
                musicInfo.m_lrcUrl = value["LrcUrl"].toString();
                musicInfo.m_smallPicUrl = value["PicUrl"].toString();
                m_musicSongInfos << musicInfo;
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

QString MusicDownLoadQueryArtistThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_WY, URL_KEY);
        case 1:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_DX, URL_KEY);
        case 2:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_QQ, URL_KEY);
        case 3:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_XM, URL_KEY);
        case 4:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_TT, URL_KEY);
        case 5:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_BD, URL_KEY);
        case 6:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_KW, URL_KEY);
        case 7:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_KG, URL_KEY);
        case 8:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_DM, URL_KEY);
        case 9:  return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_MG, URL_KEY);
        case 10: return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_MU, URL_KEY);
        case 11: return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_EC, URL_KEY);
        case 12: return MusicCryptographicHash::decryptData(MUSIC_ARTIST_MULTI_YY, URL_KEY);
    }
    return QString();
}

void MusicDownLoadQueryArtistThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info,
                                          const QString &size, int bit, const QString &url)
{
    if(!url.isEmpty())
    {
        QStringList list = url.split("/");
        if(list.isEmpty())
        {
            return;
        }

        QString lastString = list.last();
        MusicObject::MusicSongAttribute songAttr;
        songAttr.m_url = url;
        songAttr.m_size = size.isEmpty() ? "- " : size;

        list = lastString.split(".");
        if(list.isEmpty())
        {
            return;
        }

        lastString = list.last();
        if(lastString.contains("?"))
        {
            lastString = lastString.split("?").front();
        }
        songAttr.m_format = lastString;
        songAttr.m_bitrate = bit;
        info.m_songAttrs << songAttr;
    }
}
