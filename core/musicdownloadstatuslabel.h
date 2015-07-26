#ifndef MUSICDOWNLOADSTATUSLABEL_H
#define MUSICDOWNLOADSTATUSLABEL_H

#include "musicdownloadquerythread.h"
#include "musiclibexportglobal.h"

class QMovie;
class QTimer;
class MusicApplication;

class MUSIC_CORE_EXPORT MusicDownloadStatusLabel : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadStatusLabel(MusicApplication *w);
    ~MusicDownloadStatusLabel();

    void musicCheckHasLrcAlready();

signals:
    void musicBgDownloadFinished();

public slots:
    void musicHaveNoLrcAlready();
    /////////////////////////////////////////////
    ///This is a slot by MusicSongSearchOnlineWidget's signal emit
    void showDownLoadInfoFor(DownLoadType);
    void showDownLoadInfoFinished(const QString&);

protected:
    MusicApplication *m_parentWidget;
    MusicDownLoadQueryThread *m_downloadLrcThread;
    QTimer* m_downloadLrcThreadTimer;
    QMovie* m_movie;

};

#endif // MUSICDOWNLOADSTATUSLABEL_H
