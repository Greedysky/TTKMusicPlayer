#ifndef MUSICVIDEOPLAYER_H
#define MUSICVIDEOPLAYER_H

#include <QWidget>
#include "../core/musiclibexportglobal.h"

class QLabel;
class QPushButton;
class QStackedWidget;
class MusicVideoView;
class MusicVideoTableWidget;
class MusicLocalSongSearchEdit;

class MUSIC_EXPORT MusicVideoPlayer : public QWidget
{
    Q_OBJECT
public:
    MusicVideoPlayer(QWidget *parent = 0);
    ~MusicVideoPlayer();

public slots:
    void backButtonClicked();
    void afterButtonClicked();
    void searchButtonClicked();
    void mvURLChanged(const QString&);

protected:
    QWidget *m_topWidget;
    QLabel *m_textLabel;
    QPushButton *m_backButton;
    QPushButton *m_afterButton;
    QPushButton *m_searchButton;
    QStackedWidget *m_stackedWidget;
    MusicVideoView *m_videoView;
    MusicVideoTableWidget *m_videoTable;
    MusicLocalSongSearchEdit* m_searchEdit;

};

#endif //MUSICVIDEOPLAYER_H

