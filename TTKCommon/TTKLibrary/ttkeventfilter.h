#ifndef TTKEVENTFILTER_H
#define TTKEVENTFILTER_H

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

#include <QObject>
#include "ttkmoduleexport.h"

/*! @brief The class of the ttk wheel event filter.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKWheelEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit TTKWheelEventFilter(QObject *parent = nullptr);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override final;

};


/*! @brief The class of the ttk mouse event filter.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKMouseEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit TTKMouseEventFilter(QObject *parent = nullptr);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override final;

};


/*! @brief The class of the ttk context menu event filter.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKContextMenuEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit TTKContextMenuEventFilter(QObject *parent = nullptr);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override final;

};

#endif // TTKEVENTFILTER_H
