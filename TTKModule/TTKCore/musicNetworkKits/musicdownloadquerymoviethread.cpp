#include "musicdownloadquerymoviethread.h"

MusicDownLoadQueryMovieThread::MusicDownLoadQueryMovieThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

void MusicDownLoadQueryMovieThread::startToSearch(const QString &movie)
{
    m_searchText = movie;
    startToPage(0);
}
