#ifndef MUSICABSTRACTFLOATWIDGET_H
#define MUSICABSTRACTFLOATWIDGET_H

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

#include <QLabel>
#include <QPropertyAnimation>

#include "musicglobaldefine.h"

/*! @brief The class of the float widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractFloatWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractFloatWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractFloatWidget();

    /*!
     * Set block animation state or not.
     */
    inline void setBlockAnimation(bool b) noexcept { m_blockAnimation = b; }

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *) override final {}
    virtual void mousePressEvent(QMouseEvent *) override final {}
    virtual void mouseMoveEvent(QMouseEvent *) override final {}
    virtual void mouseReleaseEvent(QMouseEvent *) override final {}
    /*!
     * Show animation in case.
     */
    void animationEnter();
    /*!
     * Show animation out case.
     */
    void animationLeave();

    QPropertyAnimation *m_animation;
    QRect m_rectEnter, m_rectLeave;
    bool m_blockAnimation;

};

#endif // MUSICABSTRACTFLOATWIDGET_H
