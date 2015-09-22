#ifndef MUSICDOWNLOADQUEUECACHE_H
#define MUSICDOWNLOADQUEUECACHE_H

#include "musicdownloadthreadabstract.h"

typedef struct DownloadData
{
    QString url;
    QString savePath;
}DownloadData;

class MusicDownloadQueueCache : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownloadQueueCache(const QString &url, const QString &save,
                                     Download_Type type, QObject *parent = 0);
    explicit MusicDownloadQueueCache(const QStringList &url, const QStringList &save,
                                     Download_Type type, QObject *parent = 0);
    ~MusicDownloadQueueCache();

    void addImageQueue(const QStringList &url, const QStringList &savePath);
    virtual void startToDownload();
    void abort();

public slots:
    virtual void downLoadFinished();
    void readyReadSlot();
    void errorSlot(QNetworkReply::NetworkError code);

protected:
    void startDownload(const QString &url);
    void startOrderImageQueue();

    bool m_isDownload;
    bool m_isAbort;
    QList<DownloadData> m_imageQueue;
    QNetworkRequest *m_request;

};

#endif // MUSICDOWNLOADQUEUECACHE_H
