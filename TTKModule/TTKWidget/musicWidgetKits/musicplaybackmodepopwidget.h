#ifndef MUSICPLAYBACKMODEPOPWIDGET_H
#define MUSICPLAYBACKMODEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicobject.h"
#include "musictoolmenuwidget.h"

/*! @brief The class of the play back mode pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayBackModePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayBackModePopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlayBackModePopWidget(QWidget *parent = nullptr);

    /*!
     * Create all widget in layout.
     */
    void initWidget();

    /*!
     * Set current play mode.
     */
    void setPlaybackMode(MusicObject::PlayMode mode);

};

#endif // MUSICPLAYBACKMODEPOPWIDGET_H
