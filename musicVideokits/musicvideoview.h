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
#include "musiclibexportglobal.h"

class MusicVideoControl;
class MusicCoreMPlayer;

class MUSIC_VIDEO_EXPORT MusicViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicViewWidget(QWidget *parent = 0);

signals:
    void setFullScreen();
    void setClick();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

};

class MUSIC_VIDEO_EXPORT MusicVideoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MusicVideoView(bool popup, QWidget *parent = 0);
    ~MusicVideoView();

    void setMedia(const QString &data);
    void resizeWindow(bool resize, QSize size);

public slots:
    void play();
    void setFullScreen();

private slots:
    void stop();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void volumnChanged(int volumn);
    void mediaChanged(const QString &data);

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    bool m_positionChanged;
    MusicViewWidget *m_videoWidget;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicVideoControl *m_videoControl;

};


#endif // MUSICVIDEOVIEW_H
