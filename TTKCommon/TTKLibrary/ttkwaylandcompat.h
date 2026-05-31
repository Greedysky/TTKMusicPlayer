#ifndef TTKWAYLANDCOMPAT_H
#define TTKWAYLANDCOMPAT_H

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

#include <QPixmap>
#include "ttkmoduleexport.h"

/*! @brief The class of the wayland screenshot.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKWaylandScreenshot : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKWaylandScreenshot(QObject *parent = nullptr);

    /*!
     * Grab all screen geometry pixmap by given rect.
     */
    QPixmap grabWindow(int height, int x = 0, int y = 0, int w = -1, int h = -1) const;

private Q_SLOTS:
    /*!
     * Close animation when tiem out.
     */
    void onResponse(uint response, const QVariantMap &results);

private:
    QPixmap m_pixmap;

};

#endif // TTKWAYLANDCOMPAT_H
