#include "musicdownloadqueryttplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryTTPlaylistThread::MusicDownLoadQueryTTPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "TTpod";
}

QString MusicDownLoadQueryTTPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryTTPlaylistThread::startSearchSong(QueryType type, const QString &playlist)
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

void MusicDownLoadQueryTTPlaylistThread::startSearchSongAll()
{

}

void MusicDownLoadQueryTTPlaylistThread::startSearchSong(const QString &playlist)
{
    Q_UNUSED(playlist);
    emit downLoadDataChanged(QString());
}

void MusicDownLoadQueryTTPlaylistThread::downLoadFinished()
{

}

void MusicDownLoadQueryTTPlaylistThread::getDetailsFinished()
{

}
