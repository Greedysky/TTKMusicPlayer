#include "musicdownloadquerykwplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWPlaylistThread::MusicDownLoadQueryKWPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kuwo";
}

QString MusicDownLoadQueryKWPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWPlaylistThread::startSearchSong(QueryType type, const QString &playlist)
{
    if(type == MusicQuery)
    {
        startSearchSong(playlist);
    }
    else
    {
        startSearchSongAll();
    }
}

void MusicDownLoadQueryKWPlaylistThread::startSearchSongAll()
{

}

void MusicDownLoadQueryKWPlaylistThread::startSearchSong(const QString &playlist)
{
    Q_UNUSED(playlist);
    emit downLoadDataChanged(QString());
}

void MusicDownLoadQueryKWPlaylistThread::downLoadFinished()
{

}

void MusicDownLoadQueryKWPlaylistThread::getDetailsFinished()
{

}
