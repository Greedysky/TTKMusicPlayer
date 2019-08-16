#ifndef MUSICLRCFLOATPLAYWIDGET_H
#define MUSICLRCFLOATPLAYWIDGET_H

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

#include "musicfloatabstractwidget.h"

class QToolButton;

/*! @brief The class of the lrc play float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatPlayWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcFloatPlayWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatPlayWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcFloatPlayWidget();

    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

public Q_SLOTS:
    /*!
     * Set current player to play or not.
     */
    void musicStatePlay();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *) override {}
    virtual void mousePressEvent(QMouseEvent *) override {}
    virtual void mouseReleaseEvent(QMouseEvent *) override {}
    /*!
     * Set current play state icon.
     */
    void setCurrentPlayState();

    QToolButton *m_musicPrevious, *m_musicNext, *m_musicKey;

};

#endif // MUSICLRCFLOATPLAYWIDGET_H
