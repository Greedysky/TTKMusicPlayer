#ifndef MUSICVIDEOVIEW_H
#define MUSICVIDEOVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QAbstractScrollArea>
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
    TTK_DECLARE_MODULE(MusicViewWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicViewWidget(QWidget *parent = nullptr);
    ~MusicViewWidget();

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
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    QTimer *m_clickedTimer;
    bool m_leftPressed;
};

/*! @brief The class of the video view widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoView : public QAbstractScrollArea
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoView)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoView(QWidget *parent = nullptr);

    ~MusicVideoView();

    /*!
     * Set video play data.
     */
    void setMedia(const QString &data);
    /*!
     * Resize widget size or not.
     */
    void resizeWindow(int width, int height);
    /*!
     * Get control bar widget.
     */
    MusicVideoControlWidget *controlBarWidget() const { return m_videoControl; }

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
     * Media aution play error.
     */
    void mediaAutionPlayError(int code);
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

    MusicViewWidget *m_videoWidget;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicVideoControlWidget *m_videoControl;
    MusicBarrageWidget *m_barrageCore;

};

#endif // MUSICVIDEOVIEW_H
