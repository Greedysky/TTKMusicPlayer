#ifndef MUSICTEXTDOWNLOADTHREAD_H
#define MUSICTEXTDOWNLOADTHREAD_H

#include "musicdownloadthreadabstract.h"

class MUSIC_EXPORT MusicTextDownLoadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicTextDownLoadThread(const QString& url, const QString& save,
                                     QObject *parent = 0);
    virtual void startToDownload();

public slots:
    virtual void downLoadFinished();

};
#endif // MUSICTEXTDOWNLOADTHREAD_H
