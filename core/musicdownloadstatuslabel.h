#ifndef MUSICDOWNLOADSTATUSLABEL_H
#define MUSICDOWNLOADSTATUSLABEL_H

#include "musicdownloadmanagerthread.h"
#include "musiclibexportglobal.h"

class QMovie;
class QTimer;
class MusicApplication;

class MUSIC_EXPORT MusicDownloadStatusLabel : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadStatusLabel(MusicApplication *w);
    ~MusicDownloadStatusLabel();

    void musicCheckHasLrcAlready();

public slots:
    void musicHaveNoLrcAlready();
    /////////////////////////////////////////////
    ///This is a slot by MusicSongSearchOnlineWidget's signal emit
    void showDownLoadInfoFor(DownLoadType);
    void showDownLoadInfoFinished(const QString&);

protected:
    MusicApplication *m_parentWidget;
    MusicDownLoadManagerThread *m_downloadLrcThread;
    QTimer* m_downloadLrcThreadTimer;
    QMovie* m_movie;

};

#endif // MUSICDOWNLOADSTATUSLABEL_H
