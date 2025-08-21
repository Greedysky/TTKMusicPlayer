#ifndef MUSICPLAYLISTQUERYCOMMENTSWIDGET_H
#define MUSICPLAYLISTQUERYCOMMENTSWIDGET_H

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

#include "musiccommentswidget.h"

/*! @brief The class of the playlist comment widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylistQueryCommentsWidget : public MusicCommentsWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylistQueryCommentsWidget(QWidget *parent = nullptr);

    /*!
     * Init widget.
     */
    virtual void initialize(bool isPain) override final;

};

#endif // MUSICPLAYLISTQUERYCOMMENTSWIDGET_H
