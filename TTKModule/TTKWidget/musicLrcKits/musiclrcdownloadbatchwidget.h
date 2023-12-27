#ifndef MUSICLRCDOWNLOADBATCHWIDGET_H
#define MUSICLRCDOWNLOADBATCHWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"

namespace Ui {
class MusicLrcDownloadBatchWidget;
}

/*! @brief The class of the the lrc batch download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcDownloadBatchTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcDownloadBatchTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcDownloadBatchTableWidget(QWidget *parent = nullptr);

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongList &items);

};



/*! @brief The class of the lrc batch download widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcDownloadBatchWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcDownloadBatchWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcDownloadBatchWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcDownloadBatchWidget();

public Q_SLOTS:
    /*!
     * Add button clicked.
     */
    void addButtonClicked();
    /*!
     * Download button clicked.
     */
    void downloadButtonClicked();

private:
    Ui::MusicLrcDownloadBatchWidget *m_ui;
    MusicSongList m_localSongs;

};

#endif // MUSICLRCDOWNLOADBATCHWIDGET_H
