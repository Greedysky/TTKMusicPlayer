#include "musicdownloadqueryalbumvipthread.h"
#include "musicsettingmanager.h"
#include "musicnumberdefine.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryAlbumVipThread::MusicDownLoadQueryAlbumVipThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryAlbumVipThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryAlbumVipThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    startSearchSong(text);
}

void MusicDownLoadQueryAlbumVipThread::startSearchSong(const QString &album)
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
    request.setRawHeader("Token", "f389fc507e71499ba5d34bce00f43461");
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

void MusicDownLoadQueryAlbumVipThread::downLoadFinished()
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
                QString songName = value["songName"].toString();
                QString singerName = value["artistName"].toString();
                QString duration = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);
                QString size = value["size"].toString();

                readFromMusicSongAttribute(musicInfo, size, MB_1000, value["flacUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_1000, value["apeUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_1000, value["wavUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_320, value["sqUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_192, value["hqUrl"].toString());
                readFromMusicSongAttribute(musicInfo, size, MB_128, value["lqUrl"].toString());

                if(musicInfo.m_songAttrs.isEmpty())
                {
                    continue;
                }
                emit createSearchedItems(songName, singerName, duration);

                musicInfo.m_albumId = value["albumName"].toString() + "<>" +
                                      value["language"].toString() + "<>" +
                                      value["company"].toString() + "<>" +
                                      value["year"].toString();
                musicInfo.m_songName = songName;
                musicInfo.m_singerName = singerName;
                musicInfo.m_timeLength = duration;
                musicInfo.m_lrcUrl = value["lrcUrl"].toString();
                musicInfo.m_smallPicUrl = value["picUrl"].toString();
                m_musicSongInfos << musicInfo;
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

QString MusicDownLoadQueryAlbumVipThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_WY, URL_KEY);
        case 2:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_QQ, URL_KEY);
        case 3:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_XM, URL_KEY);
        case 4:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_TT, URL_KEY);
        case 5:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_BD, URL_KEY);
        case 6:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_KW, URL_KEY);
        case 7:  return MusicCryptographicHash::decryptData(MUSIC_ALBUM_MULTI_VIP_KG, URL_KEY);
    }
    return QString();
}

void MusicDownLoadQueryAlbumVipThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info,
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
