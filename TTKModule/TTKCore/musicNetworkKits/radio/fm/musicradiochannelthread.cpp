#include "musicradiochannelthread.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

MusicRadioChannelThread::MusicRadioChannelThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioChannelThread::~MusicRadioChannelThread()
{
    deleteAll();
}

void MusicRadioChannelThread::startToDownload(const QString &id)
{
    Q_UNUSED(id);
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(RADIO_CHANNEL_URL, false)));
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

void MusicRadioChannelThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        m_channels.clear();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            const QVariantList &channels = value["channel_list"].toList();

            QFile arcFile(":/data/fmarclist");
            arcFile.open(QFile::ReadOnly);

            QStringList arcs = QString(arcFile.readAll()).remove("\r").split("\n");
            arcFile.close();

            while(channels.count() > arcs.count())
            {
                arcs.append(QString());
            }

            for(int i=0; i<channels.count(); ++i)
            {
                value = channels[i].toMap();
                MusicRadioChannelInfo channel;
                channel.m_id = value["channel_id"].toString();
                channel.m_name = value["channel_name"].toString();
                channel.m_coverUrl = arcs[i];
                m_channels << channel;
            }
        }
    }

    emit downLoadDataChanged("query finished!");
    deleteAll();
}
