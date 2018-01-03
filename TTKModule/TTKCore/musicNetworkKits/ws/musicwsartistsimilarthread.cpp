#include "musicwsartistsimilarthread.h"
#include "musicnumberdefine.h"

#include <QTimer>

MusicWSArtistSimilarThread::MusicWSArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

QString MusicWSArtistSimilarThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSArtistSimilarThread::startToSearch(const QString &text)
{
    Q_UNUSED(text);
    QTimer::singleShot(MT_MS, this, SLOT(downLoadFinished()));
}

void MusicWSArtistSimilarThread::downLoadFinished()
{
    emit downLoadDataChanged(QString());
    deleteAll();
}
