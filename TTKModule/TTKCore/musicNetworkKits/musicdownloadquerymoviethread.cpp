#include "musicdownloadquerymoviethread.h"

MusicDownLoadQueryMovieThread::MusicDownLoadQueryMovieThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryMovieThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryMovieThread::startToSearch(const QString &movie)
{
    m_searchText = movie;
    startToPage(0);
}
