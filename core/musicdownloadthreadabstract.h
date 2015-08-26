#ifndef MUSICDOWNLOADTHREADABSTRACT_H
#define MUSICDOWNLOADTHREADABSTRACT_H

#include <QNetworkReply>
#include <QFile>
#include "musiclibexportglobal.h"

class QNetworkAccessManager;

class MUSIC_CORE_EXPORT MusicDownLoadThreadAbstract : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownLoadThreadAbstract(const QString &url, const QString &save,
                                         QObject *parent = 0);
    void deleteAll();
    virtual void startToDownload() = 0;

signals:
    void musicDownLoadFinished(const QString &name);

public slots:
    virtual void downLoadFinished() = 0;
    void replyError(QNetworkReply::NetworkError error);

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply* m_reply;
    QFile *m_file;
    QString m_url;
    QString m_savePathName;

};

#endif // MUSICDOWNLOADTHREADABSTRACT_H
