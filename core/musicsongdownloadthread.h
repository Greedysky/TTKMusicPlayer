#ifndef MUSICSONGDOWNLOADTHREAD_H
#define MUSICSONGDOWNLOADTHREAD_H

#include "musicdownloadthreadabstract.h"

class MUSIC_EXPORT MusicSongDownloadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicSongDownloadThread(const QString& url, const QString& save,
                                     QObject *parent = 0);

    virtual void startToDownload();

public slots:
    virtual void downLoadFinished();
    void downLoadReadyRead();

protected:
    void startRequest(const QUrl&);

};

#endif // MUSICSONGDOWNLOADTHREAD_H
