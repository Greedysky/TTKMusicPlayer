#ifndef MUSICLRCDOWNLOADTHREAD_H
#define MUSICLRCDOWNLOADTHREAD_H

#include <QObject>
#include <QNetworkReply>
#include <QFile>
#include "musiclibexportglobal.h"

class QNetworkAccessManager;

class MUSIC_EXPORT MusicLrcDownLoadThread : public QObject
{
    Q_OBJECT
public:
    explicit MusicLrcDownLoadThread(const QString& url, QString savePathName,
                                    QObject *parent = 0);
    void deleteAll();
    virtual void startRequest(const QUrl&);

signals:
    void musicDownLoadFinished(const QString&);

public slots:
    virtual void downLoadFinished();
    void replyError(QNetworkReply::NetworkError);

protected:
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QFile* m_file;

};
#endif // MUSICLRCDOWNLOADTHREAD_H
