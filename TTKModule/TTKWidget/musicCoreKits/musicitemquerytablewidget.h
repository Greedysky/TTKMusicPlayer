#ifndef MUSICITEMQUERYTABLEWIDGET_H
#define MUSICITEMQUERYTABLEWIDGET_H

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

#include "musicitemsearchtablewidget.h"

/*! @brief The class of the query item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemQueryTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicItemQueryTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicItemQueryTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicItemQueryTableWidget();

    /*!
     * Start to search query by text.
     */
    virtual void startToSearchByText(const QString &text) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override;

    /*!
     * Download data from net and just play or not.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download batch data from net.
     */
    void downloadBatchData();
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
    /*!
     * Table widget item cell double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Create search result items.
     */
    virtual void createResultItem(const MusicResultInfoItem &result) override;
    /*!
     * Menu action type changed.
     */
    void menuActionChanged(QAction *action);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Add search media to play list by index.
     */
    void addSearchMusicToPlaylist(int row, bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(TTK::MusicSongInformation *info, bool play);

};

#endif // MUSICITEMQUERYTABLEWIDGET_H
