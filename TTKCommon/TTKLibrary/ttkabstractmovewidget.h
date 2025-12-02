#ifndef TTKABSTRACTMOVEWIDGET_H
#define TTKABSTRACTMOVEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QWidget>
#include <QMouseEvent>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk moving widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractMoveWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKAbstractMoveWidget(QWidget *parent = nullptr);
    explicit TTKAbstractMoveWidget(bool transparent, QWidget *parent = nullptr);

    /*!
     * Set or not draw window bound shadow.
     */
    inline void drawWindowShadow(bool show) noexcept { m_shadowOption = show; }
    /*!
     * Set or not block widget moving.
     */
    inline void blockOption(bool block) noexcept { m_blockOption = block; }

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    QPoint m_mousePressedAt;
    bool m_blockOption, m_shadowOption;
    bool m_mouseLeftPressed;

};

#endif // TTKABSTRACTMOVEWIDGET_H
