#include "musicdownloadqueryqqcommentsthread.h"
#include "musicdownloadqueryqqthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQCommentsThread::MusicDownLoadQueryQQCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "QQ";
}

MusicDownLoadQueryQQCommentsThread::~MusicDownLoadQueryQQCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryQQCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQCommentsThread::startSearchSong(QueryType type, const QString &name)
{
    Q_UNUSED(type);
    Q_UNUSED(name);
}

void MusicDownLoadQueryQQCommentsThread::startSearchSong(int offset)
{
    deleteAll();
    m_pageTotal = 0;
}

void MusicDownLoadQueryQQCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
