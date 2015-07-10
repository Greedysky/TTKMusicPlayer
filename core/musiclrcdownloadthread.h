#ifndef MUSICLRCDOWNLOADTHREAD_H
#define MUSICLRCDOWNLOADTHREAD_H

#include "musicdownloadthreadabstract.h"

class MUSIC_EXPORT MusicLrcDownLoadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcDownLoadThread(const QString& url, const QString& save,
                                    QObject *parent = 0);
    virtual void startToDownload();

public slots:
    virtual void downLoadFinished();

};
#endif // MUSICLRCDOWNLOADTHREAD_H
