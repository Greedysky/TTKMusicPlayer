#include "musicfmradiosongsthread.h"
#include "musicnumberutils.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicFMRadioSongsThread::MusicFMRadioSongsThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicFMRadioThreadAbstract(parent, cookie)
{

}

MusicFMRadioSongsThread::~MusicFMRadioSongsThread()
{
    deleteAll();
}

void MusicFMRadioSongsThread::startToDownload(const QString &id)
{
    m_cachedID = id;
    m_songInfo = MusicObject::MusicSongInformation();
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(FM_SONG_URL, false).arg(id)));
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicFMRadioSongsThread::downLoadFinished()
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
            const QVariantList &songLists = value["song"].toList();
            if(songLists.isEmpty())
            {
                TTK_LOGGER_ERROR("The fm radio song is empty");
                deleteAll();
                startToDownload(m_cachedID);
                return;
            }

            foreach(const QVariant &var, songLists)
            {
                value = var.toMap();
                if(value.isEmpty() || value["url"].toString().isEmpty())
                {
                    continue;
                }

                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["url"].toString();
                attr.m_bitrate = value["kbps"].toInt();
                attr.m_format = value["file_ext"].toString();

                const QVariantMap &formats = value["available_formats"].toMap();
                if(formats.contains(QString::number(attr.m_bitrate)))
                {
                    attr.m_size = MusicUtils::Number::size2Label(formats[QString::number(attr.m_bitrate)].toInt() * 1000);
                }

                m_songInfo.m_songAttrs << attr;
                m_songInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());
                m_songInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["artist"].toString());
                m_songInfo.m_smallPicUrl = value["picture"].toString();
                m_songInfo.m_albumName = MusicUtils::String::illegalCharactersReplaced(value["albumtitle"].toString());
                m_songInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(FM_LRC_URL, false).arg(value["sid"].toString()).arg(value["ssid"].toString());
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
