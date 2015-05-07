#ifndef MUSICSONGDOWNLOADTHREAD_H
#define MUSICSONGDOWNLOADTHREAD_H

#include "musiclrcdownloadthread.h"
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicSongDownloadThread : public MusicLrcDownLoadThread
{
    Q_OBJECT
public:
    explicit MusicSongDownloadThread(const QString& url, QString savePathName,
                                     QObject *parent = 0);
    virtual void startRequest(const QUrl&);

public slots:
    virtual void downLoadFinished();
    void downLoadReadyRead();

};

#endif // MUSICSONGDOWNLOADTHREAD_H
