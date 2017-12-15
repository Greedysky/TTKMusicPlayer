#include "musicdownloadquerywsalbumthread.h"
#include "musicnumberdefine.h"

#include <QTimer>

MusicDownLoadQueryWSAlbumThread::MusicDownLoadQueryWSAlbumThread(const QString &type, QObject *parent)
    : MusicDownLoadQueryAlbumThread(parent)
{
    m_queryServer = "WuSing";
    m_querySearchType = type;
}

QString MusicDownLoadQueryWSAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSAlbumThread::startToSearch(const QString &album)
{
    Q_UNUSED(album);
    QTimer::singleShot(MT_MS, this, SLOT(downLoadFinished()));
}

void MusicDownLoadQueryWSAlbumThread::downLoadFinished()
{
    emit downLoadDataChanged(QString());
    deleteAll();
}
