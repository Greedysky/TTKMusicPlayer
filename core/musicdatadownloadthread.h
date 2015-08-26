#ifndef MUSICDATADOWNLOADTHREAD_H
#define MUSICDATADOWNLOADTHREAD_H

#include "musicdownloadthreadabstract.h"

class MUSIC_CORE_EXPORT MusicDataDownloadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDataDownloadThread(const QString &url, const QString &save,
                                     QObject *parent = 0);
    virtual void startToDownload();

public slots:
    virtual void downLoadFinished();
    void downLoadReadyRead();

protected:
    void startRequest(const QUrl &url);

};

#endif // MUSICDATADOWNLOADTHREAD_H
