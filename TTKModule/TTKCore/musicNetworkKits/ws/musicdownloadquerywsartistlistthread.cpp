#include "musicdownloadquerywsartistlistthread.h"
#include "musicnumberdefine.h"
#///QJson import
#include "qjson/parser.h"

#include <QTimer>

MusicDownLoadQueryWSArtistListThread::MusicDownLoadQueryWSArtistListThread(QObject *parent)
    : MusicDownLoadQueryArtistListThread(parent)
{
    m_pageSize = 100;
    m_queryServer = "WuSing";
}

QString MusicDownLoadQueryWSArtistListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSArtistListThread::startToPage(int offset)
{
    Q_UNUSED(offset);
    QTimer::singleShot(MT_MS, this, SLOT(downLoadFinished()));
}

void MusicDownLoadQueryWSArtistListThread::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicDownLoadQueryWSArtistListThread::downLoadFinished()
{

}
