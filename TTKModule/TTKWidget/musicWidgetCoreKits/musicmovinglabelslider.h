#ifndef MUSICMOVINGLABELSLIDER_H
#define MUSICMOVINGLABELSLIDER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QLabel>
#include "musicglobaldefine.h"
#include "musicclickedslider.h"

/*! @brief The class of the slider that can show such as tooltip.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMovingLabelSlider : public MusicClickedSlider
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMovingLabelSlider(QWidget *parent = 0);
    /*!
     * Object contsructor by slider orientation.
     */
    explicit MusicMovingLabelSlider(Qt::Orientation orientation, QWidget *parent = 0);

    ~MusicMovingLabelSlider();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Slider is now moving or not.
     */
    inline bool isMoving() const { return m_isMoving;}
    /*!
     * Set slider moving or not.
     */
    inline void setMoving(bool m) { m_isMoving = m;}

Q_SIGNALS:
    /*!
     * Slider handle release at pos emit.
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
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Set the tool tip gemetry.
     */
    QPoint limitLableGeometry(int x, int y, int z);

    bool m_isMoving;
    QLabel *m_textLabel;
    Qt::Orientation m_orientation;

};

#endif // MUSICMOVINGLABELSLIDER_H
