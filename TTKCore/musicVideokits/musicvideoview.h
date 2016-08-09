#ifndef MUSICVIDEOVIEW_H
#define MUSICVIDEOVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QGraphicsView>
#include "musicglobaldefine.h"
#include "musicvideocontrol.h"

class MusicCoreMPlayer;
class MusicBarrageWidget;

/*! @brief The class of the video show widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicViewWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void setFullScreen();
    /*!
     * Widget double clicked.
     */
    void setClick();
    /*!
     * Widget clicked.
     */
    void mediaIsPlaying(bool &play);
    /*!
     * Current media is playing.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
};

/*! @brief The class of the video view widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MusicVideoView(bool popup, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicVideoView();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setMedia(const QString &data);
    /*!
     * Set video play data.
     */
    void resizeWindow(int width, int height);
    /*!
     * Resize widget size or not.
     */

public Q_SLOTS:
    void play();
    /*!
     * Player to play.
     */

private Q_SLOTS:
    void stop();
    /*!
     * Player to stop.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void setPosition(int position);
    /*!
     * Set current play pos.
     */
    void volumeChanged(int volume);
    /*!
     * Current volume changed.
     */
    void mediaChanged(const QString &data);
    /*!
     * Current media changed.
     */
    void mediaIsPlaying(bool &play);
    /*!
     * Current media is playing.
     */

    void addBarrageChanged(const MusicBarrageRecord &record);
    /*!
     * Add barrage text to mv.
     */
    void pushBarrageChanged(bool on);
    /*!
     * Open barrage on or not.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    bool m_positionChanged;
    MusicViewWidget *m_videoWidget;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicVideoControl *m_videoControl;
    MusicBarrageWidget *m_barrageCore;

};


#endif // MUSICVIDEOVIEW_H
