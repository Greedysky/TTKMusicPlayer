#ifndef MUSICQUERYITEMTABLEWIDGET_H
#define MUSICQUERYITEMTABLEWIDGET_H

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

#include <QMenu>
#include <QActionGroup>
#include "musicnetworkthread.h"
#include "musicfillitemtablewidget.h"
#include "musicdownloadquerythreadabstract.h"

class MusicGifLabelWidget;

/*! @brief The class of the query table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryTableWidget(QWidget *parent = nullptr);

    virtual ~MusicQueryTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Get network query input.
     */
    MusicDownLoadQueryThreadAbstract *getQueryInput();
    /*!
     * Start search query by text.
     * Subclass should implement this function.
     */
    virtual void startSearchQuery(const QString &text) = 0;
    /*!
     * Data download to local file.
     * Subclass should implement this function.
     */
    virtual void musicDownloadLocal(int row) = 0;

Q_SIGNALS:
    /*!
     * Add current network music to download to local.
     */
    void musicSongToPlaylistChanged(const QString &name, const QString &time, const QString &format, bool play);
    /*!
     * Show download state type.
     */
    void showDownLoadInfoFor(MusicObject::DownLoadMode type);
    /*!
     * Restart search query by name.
     */
    void restartSearchQuery(const QString &name);

public Q_SLOTS:
    /*!
     * Clear All Items.
     * Subclass should implement this function.
     */
    virtual void clearAllItems() = 0;
    /*!
     * Create searched items.
     * Subclass should implement this function.
     */
    virtual void createSearchedItem(const MusicSearchedItem &songItem) = 0;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    MusicDownLoadQueryThreadAbstract *m_downLoadManager;

};


class MusicLabelDelegate;
/*! @brief The class of the query item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQueryItemTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryItemTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryItemTableWidget(QWidget *parent = nullptr);

    virtual ~MusicQueryItemTableWidget();

    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Left context menu action group click by action.
     */
    virtual void actionGroupClick(QAction *action);
    /*!
     * Item has double clicked.
     * Subclass should implement this function.
     */
    virtual void itemDoubleClicked(int row, int column) = 0;
    /*!
     * Create the search finished item.
     */
    void createFinishedItem();

protected:
    /*!
     * Create context menu.
     */
    void createContextMenu(QMenu &menu);
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    /*!
     * Get current random video hotter(simulate).
     */
    QString randToGetStrength() const;

    MusicGifLabelWidget *m_loadingLabel;
    QActionGroup *m_actionGroup;
    MusicLabelDelegate *m_labelDelegate;

};

#endif // MUSICQUERYITEMTABLEWIDGET_H
