#ifndef MUSICDOWNLOADSTATUSLABEL_H
#define MUSICDOWNLOADSTATUSLABEL_H

#include "musicdownloadquerythread.h"
#include "musiclibexportglobal.h"

class QLabel;
class QMovie;
class QTimer;
class MusicApplication;

class MUSIC_WIDGET_EXPORT MusicDownloadStatusLabel : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadStatusLabel(QWidget *w);
    ~MusicDownloadStatusLabel();

    void setMovieLabel(QLabel *label) { m_movieLabel = label;}
    void musicCheckHasLrcAlready();
    bool checkSettingParameterValue();

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
    QLabel* m_movieLabel;

};

#endif // MUSICDOWNLOADSTATUSLABEL_H
