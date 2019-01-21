#ifndef MUSICLRCDOWNLOADBATCHWIDGET_H
#define MUSICLRCDOWNLOADBATCHWIDGET_H

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

#include "musicsong.h"
#include "musicabstractmovewidget.h"
#include "musicabstracttablewidget.h"

namespace Ui {
class MusicLrcDownloadBatchWidget;
}
class MusicDownloadQueueCache;

/*! @brief The class of the the lrc batch download table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLrcDownloadBatchTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcDownloadBatchTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcDownloadBatchTableWidget(QWidget *parent = nullptr);

    /*!
     * Create all items.
     */
    void createAllItems(const MusicSongs &items);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
};



/*! @brief The class of the lrc batch download widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcDownloadBatchWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcDownloadBatchWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcDownloadBatchWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcDownloadBatchWidget();

Q_SIGNALS:
    /*!
     * Get music datas from container.
     */
    void getMusicLists(MusicSongItems &songs);

public Q_SLOTS:
    /*!
     * Modified item button clicked.
     */
    void modifiedItemButtonClicked();
    /*!
     * Item lists changed.
     */
    void itemListsChanged(const MIntList &items);
    /*!
     * Add button clicked.
     */
    void addButtonClicked();
    /*!
     * Download button clicked.
     */
    void downloadButtonClicked();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Get selected song items.
     */
    void getSelectedSongItems();

    Ui::MusicLrcDownloadBatchWidget *m_ui;

    MusicSongs m_localSongs;
    bool m_selectedItemIdFlag;
    MIntList m_selectedItemIds;

};

#endif // MUSICLRCDOWNLOADBATCHWIDGET_H
