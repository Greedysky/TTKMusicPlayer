#include "musicradiochannelthread.h"

#ifdef MUSIC_GREATER_NEW
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

QString MusicRadioChannelThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicRadioChannelThread::startToDownload(const QString &)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(channelUrl));
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

ChannelInfos MusicRadioChannelThread::getMusicChannel()
{
    return m_channels;
}

void MusicRadioChannelThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        m_channels.clear();
#ifdef MUSIC_GREATER_NEW
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
    emit downLoadDataChanged("query finished!");
    deleteAll();
}
