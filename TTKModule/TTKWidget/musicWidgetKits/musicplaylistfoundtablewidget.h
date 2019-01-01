#ifndef MUSICPLAYLISTFOUNDTABLEWIDGET_H
#define MUSICPLAYLISTFOUNDTABLEWIDGET_H

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
#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the playlist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylistFoundTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundTableWidget(QWidget *parent = nullptr);

    virtual ~MusicPlaylistFoundTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query) override;
    /*!
     * Set connect object.
     */
    void setConnectObject(QObject *obj);

};

#endif // MUSICPLAYLISTFOUNDTABLEWIDGET_H
