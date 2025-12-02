#ifndef MUSICMOVINGCLICKEDSLIDER_H
#define MUSICMOVINGCLICKEDSLIDER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#include "ttkclickedslider.h"

/*! @brief The class of the slider moving clicked slider.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMovingClickedSlider : public TTKClickedSlider
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicMovingClickedSlider(QWidget *parent = nullptr);
    explicit MusicMovingClickedSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

    /*!
     * Slider is now moving or not.
     */
    inline bool isMoving() const noexcept { return m_isMoving; }
    /*!
     * Set slider moving or not.
     */
    inline void setMoving(bool m) noexcept { m_isMoving = m; }

Q_SIGNALS:
    /*!
     * Slider handle release at pos.
     */
    void sliderReleasedAt(int pos);

public Q_SLOTS:
    /*!
     * Set slider current value.
     */
    void setValue(int value);

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    bool m_isMoving;

};

#endif // MUSICMOVINGCLICKEDSLIDER_H
