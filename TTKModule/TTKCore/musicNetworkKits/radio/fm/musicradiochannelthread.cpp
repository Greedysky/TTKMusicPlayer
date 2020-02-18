#include "musicradiochannelthread.h"
#///QJson import
#include "qjson/parser.h"
#///Oss import
#include "qoss/qossconf.h"

#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

#define OS_RADIO_URL  "BaiduRadio"

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

            for(int i=0; i<channels.count(); ++i)
            {
                value = channels[i].toMap();
                MusicRadioChannelInfo channel;
                channel.m_id = value["channel_id"].toString();
                channel.m_name = value["channel_name"].toString();
                channel.m_coverUrl = QOSSConf::generateDataBucketUrl() + QString("%1/%2%3").arg(OS_RADIO_URL).arg(i + 1).arg(JPG_FILE);
                m_channels << channel;
            }
        }
    }

    Q_EMIT downLoadDataChanged("query finished!");
    deleteAll();
}
