#ifndef MUSICDATADOWNLOADTHREAD_H
#define MUSICDATADOWNLOADTHREAD_H

#include "musicsongdownloadthread.h"

class MUSIC_EXPORT MusicDataDownloadThread : public MusicSongDownloadThread
{
    Q_OBJECT
public:
    explicit MusicDataDownloadThread(const QString& url, const QString& save,
                                     QObject *parent = 0);

    void deleteAll();
    virtual void startToDownload();

public slots:
    void dataGetFinished();
    void dataReplyError(QNetworkReply::NetworkError);

protected:
    QNetworkAccessManager* m_dataManager;
    QNetworkReply* m_dataReply;

};

#endif // MUSICDATADOWNLOADTHREAD_H
