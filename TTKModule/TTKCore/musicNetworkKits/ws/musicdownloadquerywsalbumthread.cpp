#include "musicdownloadquerywsalbumthread.h"

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
    QTimer::singleShot(1, this, SLOT(downLoadFinished()));
}

void MusicDownLoadQueryWSAlbumThread::downLoadFinished()
{
    emit downLoadDataChanged(QString());
    deleteAll();
}
