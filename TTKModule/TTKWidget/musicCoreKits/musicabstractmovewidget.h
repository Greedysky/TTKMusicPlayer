#ifndef MUSICABSTRACTMOVEWIDGET_H
#define MUSICABSTRACTMOVEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicwidgetrenderer.h"
#include "ttkabstractmovewidget.h"

/*! @brief The class of the moving widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractMoveWidget : public TTKAbstractMoveWidget, protected MusicWidgetRenderer
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractMoveWidget(QWidget *parent = nullptr);
    explicit MusicAbstractMoveWidget(bool transparent, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractMoveWidget();

public Q_SLOTS:
    /*!
     * Background image changed.
     */
    void backgroundChanged();
    /*!
     * Override show function.
     */
    void show();

};


/*! @brief The class of the moving widget with plain.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractMovePlainWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractMovePlainWidget(QWidget *parent = nullptr);
    explicit MusicAbstractMovePlainWidget(bool transparent, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractMovePlainWidget();

protected:
    QWidget *m_container;

};

#endif // MUSICABSTRACTMOVEWIDGET_H
