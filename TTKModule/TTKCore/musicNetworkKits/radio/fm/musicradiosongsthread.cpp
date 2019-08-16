#include "musicradiosongsthread.h"
#include "musicnumberutils.h"
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

void MusicRadioSongsThread::startToDownload(const QString &id)
{
    m_songInfo = MusicObject::MusicSongInformation();
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(RADIO_SONG_URL, false) + id));
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
    MusicObject::setSslConfiguration(&request);
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

void MusicRadioSongsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["data"].toMap();
            const QVariantList &songLists = value["songList"].toList();
            foreach(const QVariant &var, songLists)
            {
                value = var.toMap();
                if(value.isEmpty() || value["songLink"].toString().isEmpty())
                {
                    continue;
                }

                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["songLink"].toString();
                attr.m_bitrate = value["rate"].toInt();
                attr.m_format = value["format"].toString();
                attr.m_size = MusicUtils::Number::size2Label(value["size"].toLongLong());

                m_songInfo.m_songAttrs << attr;
                m_songInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["songName"].toString());
                m_songInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["artistName"].toString());
                m_songInfo.m_smallPicUrl = value["songPicRadio"].toString();
                m_songInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumName"].toString());
                m_songInfo.m_lrcUrl = value["lrcLink"].toString();

                if(!m_songInfo.m_lrcUrl.contains("http://"))
                {
                    m_songInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(RADIO_LRC_URL, false) + m_songInfo.m_lrcUrl;
                }
            }
        }
    }

    emit downLoadDataChanged("query finished!");
    deleteAll();
}
