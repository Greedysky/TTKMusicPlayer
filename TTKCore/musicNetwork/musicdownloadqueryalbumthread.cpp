#include "musicdownloadqueryalbumthread.h"
#include "musicsettingmanager.h"

MusicDownLoadQueryAlbumThread::MusicDownLoadQueryAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryAlbumThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    Q_UNUSED(text);
}

void MusicDownLoadQueryAlbumThread::startSearchSong(const QString &album)
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

void MusicDownLoadQueryAlbumThread::downLoadFinished()
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
#ifdef MUSIC_GREATER_NEW
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        qDebug() << bytes;
        emit downLoadDataChanged(QString());
#else

#endif
    }
}

QString MusicDownLoadQueryAlbumThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MUSIC_ALBUM_WY;
        case 1:  return MUSIC_ALBUM_DX;
        case 2:  return MUSIC_ALBUM_QQ;
        case 3:  return MUSIC_ALBUM_XM;
        case 4:  return MUSIC_ALBUM_TT;
        case 5:  return MUSIC_ALBUM_BD;
        case 6:  return MUSIC_ALBUM_KW;
        case 7:  return MUSIC_ALBUM_KG;
        case 8:  return MUSIC_ALBUM_DM;
        case 9:  return MUSIC_ALBUM_MG;
        case 10: return MUSIC_ALBUM_MU;
        case 11: return MUSIC_ALBUM_EC;
        case 12: return MUSIC_ALBUM_YY;
    }
    return QString();
}
