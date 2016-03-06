#include "musicradiochannelthread.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>

MusicRadioChannelThread::MusicRadioChannelThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioChannelThread::~MusicRadioChannelThread()
{
    deleteAll();
}

void MusicRadioChannelThread::startToDownload(const QString &)
{
    m_manager = new QNetworkAccessManager(this);
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(channelUrl));
    if(m_cookJar)
    {
        m_manager->setCookieJar(m_cookJar);
    }
    m_reply = m_manager->get(networkRequest);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));

}

ChannelInfos MusicRadioChannelThread::getMusicChannel()
{
    return m_channels;
}

void MusicRadioChannelThread::downLoadFinished()
{
    QByteArray bytes = m_reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
    ///Put the data into Json
    if(jsonError.error != QJsonParseError::NoError ||
       !parseDoucment.isObject())
    {
        deleteAll();
        return ;
    }

    m_channels.clear();
    QJsonObject jsonObject = parseDoucment.object();
    if(jsonObject.contains("channel_list"))
    {
        QJsonArray array = jsonObject.take("channel_list").toArray();
        foreach(QJsonValue value, array)
        {
            if(!value.isObject())
            {
               continue;
            }
            QJsonObject object = value.toObject();

            ChannelInfo channel;
            channel.m_id = object.value("channel_id").toString();
            channel.m_name = object.value("channel_name").toString();
            m_channels << channel;
        }
    }
    emit networkReplyFinished("query finished!");
    deleteAll();
}
