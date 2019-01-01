#ifndef MUSICSONGSLISTPLAYEDTABLEWIDGET_H
#define MUSICSONGSLISTPLAYEDTABLEWIDGET_H

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

#include "musicsongslistabstracttablewidget.h"

class MusicSongsListPlayedWidget;

/*! @brief The class of the songs played queue list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListPlayedTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListPlayedTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListPlayedTableWidget(QWidget *parent = nullptr);

    virtual ~MusicSongsListPlayedTableWidget();

    /*!
     * Clear play later state.
     */
    void clearPlayLaterState();
    /*!
     * Set play later state.
     */
    void setPlayLaterState(int row);
    /*!
     * Update songs file names in table.
     */
    virtual void updateSongsFileName(const MusicSongs &songs) override;

    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Select the current play row by given index.
     */
    virtual void selectRow(int index) override;
    /*!
     * Select the current played row.
     */
    void selectPlayedRow();
    /*!
     * Replace current play widget row.
     */
    void replacePlayWidgetRow();

Q_SIGNALS:
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAt(int index);

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Delete item from list at current row.
     */
    virtual void setDeleteItemAt() override;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    MusicSongsListPlayedWidget *m_musicSongsPlayWidget;

};

#endif // MUSICSONGSLISTPLAYEDTABLEWIDGET_H
