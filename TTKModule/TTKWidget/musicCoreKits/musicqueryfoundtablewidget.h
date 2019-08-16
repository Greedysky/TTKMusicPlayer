#ifndef MUSICQUERYFOUNDTABLEWIDGET_H
#define MUSICQUERYFOUNDTABLEWIDGET_H

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

#include "musicquerytablewidget.h"

/*! @brief The class of the query found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryFoundTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryFoundTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryFoundTableWidget(QWidget *parent = nullptr);

    virtual ~MusicQueryFoundTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Data download to local file.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Download data from net and just play or not.
     */
    void downloadDataFrom(bool play);
    /*!
     * Download batch data from net.
     */
    void downloadBatchData(bool music);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Subclass should implement this function.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItem(const MusicSearchedItem &songItem) override;
    /*!
     * Create the search finished item.
     */
    void createFinishedItem();
    /*!
     * Search menu type changed.
     */
    void searchChanged(QAction *action);

protected:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    /*!
     * Add search music to play list by index.
     */
    void addSearchMusicToPlaylist(int row, bool play);
    /*!
     * Download data from net and just play or not.
     */
    bool downloadDataFrom(const MusicObject::MusicSongInformation &downloadInfo, bool play);

    MusicLabelDelegate *m_labelDelegate;

};

#endif // MUSICQUERYFOUNDTABLEWIDGET_H
