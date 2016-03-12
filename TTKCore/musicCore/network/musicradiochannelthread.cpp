#include "musicradiochannelthread.h"

#ifdef MUSIC_QT_5
#   include <QJsonParseError>
#   include <QJsonDocument>
#   include <QJsonObject>
#   include <QJsonArray>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
#endif

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
        m_cookJar->setParent(nullptr);
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
    if(m_reply == nullptr)
    {
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        m_channels.clear();
#ifdef MUSIC_QT_5
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            deleteAll();
            return ;
        }

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
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(bytes));
        if(sc.property("channel_list").isArray())
        {
            QScriptValueIterator it(sc.property("channel_list"));
            while(it.hasNext())
            {
                it.next();
                QScriptValue value = it.value();
                if(value.isNull() || value.property("channel_id").toString().isEmpty())
                {
                    continue;
                }

                ChannelInfo channel;
                channel.m_id = value.property("channel_id").toString();
                channel.m_name = value.property("channel_name").toString();
                m_channels << channel;
            }
        }
#endif
    }
    emit networkReplyFinished("query finished!");
    deleteAll();
}
