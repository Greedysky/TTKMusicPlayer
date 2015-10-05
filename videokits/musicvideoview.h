#ifndef MUSICVIDEOVIEW_H
#define MUSICVIDEOVIEW_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QGraphicsView>
#include <QMediaPlayer>
#include "musiclibexportglobal.h"

class MusicVideoControl;
class QGraphicsVideoItem;

class MUSIC_VIDEO_EXPORT MusicVideoView : public QGraphicsView
{
    Q_OBJECT
public:
    MusicVideoView(bool popup, QWidget *parent = 0);
    virtual ~MusicVideoView();

    void setMedia(const QString &data);
    void resizeWindow(bool resize, QSize size);

public slots:
    void play();

private slots:
    void stop();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void volumnChanged(int volumn);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

    bool m_positionChanged;
    QMediaPlayer m_mediaPlayer;
    QGraphicsVideoItem *m_videoItem;
    MusicVideoControl *m_videoControl;

};


#endif // MUSICVIDEOVIEW_H
