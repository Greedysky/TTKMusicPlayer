#include "musicdownloadquerybdcommentsthread.h"
#include "musicdownloadquerybdthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDCommentsThread::MusicDownLoadQueryBDCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "Baidu";
}

MusicDownLoadQueryBDCommentsThread::~MusicDownLoadQueryBDCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryBDCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDCommentsThread::startSearchSong(QueryType type, const QString &name)
{
    Q_UNUSED(type);

    MusicSemaphoreLoop loop;
    MusicDownLoadQueryBDThread *query = new MusicDownLoadQueryBDThread(this);
    query->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startSearchSong(0);
    }
}

void MusicDownLoadQueryBDCommentsThread::startSearchSong(int offset)
{
    deleteAll();
    m_pageTotal = 0;
}

void MusicDownLoadQueryBDCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {

    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
