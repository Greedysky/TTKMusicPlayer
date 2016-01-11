#include "musicdownloadquerymultiplethread.h"
#include "musicdownloadthreadabstract.h"

#ifdef MUSIC_QT_5
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
#endif
#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryMultipleThread::MusicDownLoadQueryMultipleThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

MusicDownLoadQueryMultipleThread::~MusicDownLoadQueryMultipleThread()
{

}

void MusicDownLoadQueryMultipleThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = (type != MovieQuery ) ? MUSIC_REQUERY_URL.arg(text) : MV_REQUERY_URL.arg(text);
    ///This is a multiple music API

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    m_reply = m_manager->get(QNetworkRequest(musicUrl));
    connect(m_reply, SIGNAL(finished()), SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryMultipleThread::searchFinshed()
{

}
