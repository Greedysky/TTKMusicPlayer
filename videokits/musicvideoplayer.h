#ifndef MUSICVIDEOPLAYER_H
#define MUSICVIDEOPLAYER_H

#include <QGraphicsView>
#include <QMediaPlayer>
#include <QModelIndex>
#include "../core/musiclibexportglobal.h"

class QMediaPlaylist;
class MusicVideoControl;
class QGraphicsVideoItem;
class MusicVideoPlayListWidget;

class MUSIC_EXPORT MusicVideoPlayer : public QGraphicsView
{
    Q_OBJECT
public:
    MusicVideoPlayer(QWidget *parent = 0);
    ~MusicVideoPlayer();

signals:
    void movieDoubleClicked();

public slots:
    void addMedia(const QStringList&);
    void play();
    void playNext();
    void playPrivious();
    void showPlayList();

private slots:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void volumnChanged(int volumn);
    void listDoubleClicked(const QModelIndex&);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QMediaPlayer m_mediaPlayer;
    QMediaPlaylist* m_mediaLists;
    QGraphicsVideoItem *m_videoItem;
    MusicVideoControl *m_videoControl;
    MusicVideoPlayListWidget *m_videoListWidget;

};

#endif //MUSICVIDEOPLAYER_H

