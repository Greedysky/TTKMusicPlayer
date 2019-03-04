#include "musicradioplaylistthread.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicRadioPlaylistThread::MusicRadioPlaylistThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioPlaylistThread::~MusicRadioPlaylistThread()
{
    deleteAll();
}

void MusicRadioPlaylistThread::startToDownload(const QString &id)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(RADIO_PLAYLIST_URL, false) + id));
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

void MusicRadioPlaylistThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        m_playlist.clear();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            const QVariantList &channels = value["list"].toList();
            foreach(const QVariant &channel, channels)
            {
                value = channel.toMap();
                m_playlist << QString::number(value["id"].toInt());
            }
        }
    }

    emit downLoadDataChanged("query finished!");
    deleteAll();
}
