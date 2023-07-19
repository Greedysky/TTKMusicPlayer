#ifndef TTKDESKTOPWRAPPER_H
#define TTKDESKTOPWRAPPER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QWidget>
#include "ttkmoduleexport.h"

namespace TTK {
enum class Direction;
}

/*! @brief The class of the desktop screen wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKDesktopWrapper
{
public:
    struct TaskbarInfo
    {
        int m_size;
        TTK::Direction m_direction;
    };

    /*!
     * Get screen taskbar info by given screen index.
     */
    static TaskbarInfo screenTaskbar(int index = 0);

    /*!
     * Get screen geometry by given screen index.
     */
    static QRect screenGeometry(int index = 0);
    /*!
     * Get all screen geometry.
     */
    static QRect geometry();

    /*!
     * Grab widget geometry pixmap by given rect.
     */
    static QPixmap grabWidget(QWidget *widget, const QRect &rect);
    /*!
     * Grab all screen geometry pixmap by given rect.
     */
    static QPixmap grabWindow(int x = 0, int y = 0, int w = -1, int h = -1);

};

#endif // TTKDESKTOPWRAPPER_H
