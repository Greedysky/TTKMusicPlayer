#include "musicdownloadquerymultiplevipthread.h"
#include "musicdownloadthreadabstract.h"
#include "musicsettingmanager.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryMultipleVipThread::MusicDownLoadQueryMultipleVipThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryMultipleVipThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryMultipleVipThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = QString(getCurrentURL()).arg(text);
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

QString MusicDownLoadQueryMultipleVipThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_WY, URL_KEY);
        case 2:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_QQ, URL_KEY);
        case 3:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_XM, URL_KEY);
        case 4:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_TT, URL_KEY);
        case 5:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_BD, URL_KEY);
        case 6:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_KW, URL_KEY);
        case 7:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_KG, URL_KEY);
    }
    return QString();
}

void MusicDownLoadQueryMultipleVipThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info,
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

void MusicDownLoadQueryMultipleVipThread::downLoadFinished()
{

}
