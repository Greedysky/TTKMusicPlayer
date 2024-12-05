#ifndef TTKABSTRACTMOVERESIZEWIDGET_H
#define TTKABSTRACTMOVERESIZEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

/*! @brief The class of the moving resize struct.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MoveResizeData
{
    bool m_borderPressed;
    bool m_mouseLeftPressed;
    QPoint m_mousePos;
    QPoint m_windowPos;
    QSize m_pressedSize;
};

namespace TTK
{
enum class Direction;
}

/*! @brief The class of the ttk moving resize widget base.
 * @author Greedysky <greedysky@163.com>
 *         Lverson   <what951006@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractMoveResizeWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKAbstractMoveResizeWidget)
public:
    /*!
     * Object constructor.
     */
    explicit TTKAbstractMoveResizeWidget(QWidget *parent = nullptr);
    explicit TTKAbstractMoveResizeWidget(bool transparent, QWidget *parent = nullptr);

protected:
    /*!
     * Override the widget event.
     */
    virtual bool eventFilter(QObject *object, QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    /*!
     * Size direction detected.
     */
    void sizeDirection();
    /*!
     * Move direction to be.
     */
    void moveDirection();
    /*!
     * set given widgets tracking.
     */
    void setObjectsTracking(const QWidgetList &objects);

    MoveResizeData m_struct;
    TTK::Direction m_direction;

};

#endif // TTKABSTRACTMOVERESIZEWIDGET_H
