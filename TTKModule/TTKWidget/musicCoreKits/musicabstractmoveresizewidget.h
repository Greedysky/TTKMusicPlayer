#ifndef MUSICABSTRACTMOVERESIZEWIDGET_H
#define MUSICABSTRACTMOVERESIZEWIDGET_H

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

#include "musicwidgetrenderer.h"
#include "ttkabstractmoveresizewidget.h"

/*! @brief The class of the moving resize widget with plain.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractMoveResizePlainWidget : public TTKAbstractMoveResizeWidget, protected MusicWidgetRenderer
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractMoveResizePlainWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractMoveResizePlainWidget();

public Q_SLOTS:
    /*!
     * Background image changed.
     */
    void backgroundChanged();
    /*!
     * Override show function.
     */
    void show();

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

};

#endif // MUSICABSTRACTMOVERESIZEWIDGET_H
