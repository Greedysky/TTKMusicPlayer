#include "musicradioplaylistthread.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

    m_playList.clear();
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
            m_playList << QString::number(object.value("id").toInt());
        }
    }
    emit networkReplyFinished("query finished!");
    deleteAll();
}
