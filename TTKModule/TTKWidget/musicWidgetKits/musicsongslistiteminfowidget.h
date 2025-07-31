#ifndef MUSICSONGSLISTITEMINFOWIDGET_H
#define MUSICSONGSLISTITEMINFOWIDGET_H

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

#include "musicsong.h"
#include "ttkabstractmovewidget.h"

namespace Ui {
class MusicSongsListItemInfoWidget;
}

/*! @brief The class of the song list item info widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsListItemInfoWidget : public TTKAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListItemInfoWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsListItemInfoWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsListItemInfoWidget();

    /*!
     * Set media song information.
     */
    void initialize(int index, const MusicSong &song);

private:
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtistPicture(const QString &name);

    Ui::MusicSongsListItemInfoWidget *m_ui;

};

#endif // MUSICSONGSLISTITEMINFOWIDGET_H
