#include "musicradiosongsthread.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicRadioSongsThread::MusicRadioSongsThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioSongsThread::~MusicRadioSongsThread()
{
    deleteAll();
}

QString MusicRadioSongsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicRadioSongsThread::startToDownload(const QString &id)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(SONG_URL, false) + id));
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    if(m_cookJar)
    {
        m_manager->setCookieJar(m_cookJar);
        m_cookJar->setParent(nullptr);
    }
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));

}

MusicRadioSongInfo MusicRadioSongsThread::getMusicSongInfo()
{
    return m_songInfo;
}

void MusicRadioSongsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["data"].toMap();
            QVariantList songLists = value["songList"].toList();
            foreach(const QVariant &var, songLists)
            {
                value = var.toMap();
                if(value.isEmpty() || value["songLink"].toString().isEmpty())
                {
                    continue;
                }

                m_songInfo.m_songUrl = value["songLink"].toString();
                m_songInfo.m_songName = value["songName"].toString();
                m_songInfo.m_artistName = value["artistName"].toString();
                m_songInfo.m_songPicUrl = value["songPicRadio"].toString();
                m_songInfo.m_albumName = value["albumName"].toString();
                m_songInfo.m_lrcUrl = value["lrcLink"].toString();

                QString lrcPrefix = MusicUtils::Algorithm::mdII(LRC_URL, false);
                if(!m_songInfo.m_lrcUrl.contains(lrcPrefix))
                {
                    m_songInfo.m_lrcUrl = lrcPrefix + m_songInfo.m_lrcUrl;
                }
            }
        }
    }
    emit downLoadDataChanged("query finished!");
    deleteAll();
}
