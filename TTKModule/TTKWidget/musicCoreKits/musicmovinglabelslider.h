#ifndef MUSICMOVINGLABELSLIDER_H
#define MUSICMOVINGLABELSLIDER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QLabel>
#include "musicuiobject.h"
#include "musicmovingclickedslider.h"

/*! @brief The class of the slider that can show such as tooltip.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMovingLabelSlider : public MusicMovingClickedSlider
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMovingLabelSlider)
public:
    /*!
     * Object constructor.
     */
    explicit MusicMovingLabelSlider(QWidget *parent = nullptr);
    explicit MusicMovingLabelSlider(Qt::Orientation orientation, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMovingLabelSlider();

private:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    /*!
     * Set the tool tip gemetry.
     */
    QPoint limitLableGeometry(int x, int y, int z);

    QLabel *m_textLabel;

};

#endif // MUSICMOVINGLABELSLIDER_H
