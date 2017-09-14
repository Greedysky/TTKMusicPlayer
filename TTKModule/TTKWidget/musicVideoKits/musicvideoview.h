#ifndef MUSICVIDEOVIEW_H
#define MUSICVIDEOVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QGraphicsView>
#include "musicglobaldefine.h"
#include "musicvideocontrolwidget.h"

class MusicCoreMPlayer;
class MusicBarrageWidget;

/*! @brief The class of the video show widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicViewWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicViewWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Widget double clicked.
     */
    void setFullScreen();
    /*!
     * Widget clicked.
     */
    void setClick();
    /*!
     * Current media is playing.
     */
    void mediaIsPlaying(bool &play);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};

/*! @brief The class of the video view widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoView : public QGraphicsView
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoView(QWidget *parent = 0);

    ~MusicVideoView();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set video play data.
     */
    void setMedia(const QString &data);
    /*!
     * Resize widget size or not.
     */
    void resizeWindow(int width, int height);

public Q_SLOTS:
    /*!
     * Player to play.
     */
    void play();

private Q_SLOTS:
    /*!
     * Player to stop.
     */
    void stop();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Set current play pos.
     */
    void setPosition(int position);
    /*!
     * Current volume changed.
     */
    void volumeChanged(int volume);
    /*!
     * Current media changed.
     */
    void mediaChanged(const QString &data);
    /*!
     * Current media is playing.
     */
    void mediaIsPlaying(bool &play);

    /*!
     * Add barrage text to mv.
     */
    void addBarrageChanged(const MusicBarrageRecord &record);
    /*!
     * Open barrage on or not.
     */
    void pushBarrageChanged(bool on);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;


    bool m_positionChanged;
    MusicViewWidget *m_videoWidget;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicVideoControlWidget *m_videoControl;
    MusicBarrageWidget *m_barrageCore;

};


#endif // MUSICVIDEOVIEW_H
