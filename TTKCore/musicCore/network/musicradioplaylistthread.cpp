#include "musicradioplaylistthread.h"

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

MusicRadioPlayListThread::MusicRadioPlayListThread(QObject *parent, QNetworkCookieJar *cookie)
    : MusicRadioThreadAbstract(parent, cookie)
{

}

MusicRadioPlayListThread::~MusicRadioPlayListThread()
{
    deleteAll();
}

void MusicRadioPlayListThread::startToDownload(const QString &id)
{
    m_manager = new QNetworkAccessManager(this);
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(playListUrl + id));
    if(m_cookJar)
    {
        m_manager->setCookieJar(m_cookJar);
        m_cookJar->setParent(nullptr);
    }
    m_reply = m_manager->get(networkRequest);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));

}

QStringList MusicRadioPlayListThread::getMusicPlayList()
{
    return m_playList;
}

void MusicRadioPlayListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();
        m_playList.clear();
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
        if(jsonObject.contains("list"))
        {
            QJsonArray array = jsonObject.value("list").toArray();
            foreach(QJsonValue value, array)
            {
                if(!value.isObject())
                {
                   continue;
                }
                QJsonObject object = value.toObject();
                m_playList << QString::number(object.value("id").toVariant().toInt());
            }
        }
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(bytes));
        if(sc.property("list").isArray())
        {
            QScriptValueIterator it(sc.property("list"));
            while(it.hasNext())
            {
                it.next();
                QScriptValue value = it.value();
                if(value.isNull())
                {
                    continue;
                }

                m_playList << QString::number(value.property("id").toInt32());
            }
        }
#endif
    }
    emit networkReplyFinished("query finished!");
    deleteAll();
}
