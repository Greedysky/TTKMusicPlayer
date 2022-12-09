#ifndef MUSICQUERYTABLEWIDGET_H
#define MUSICQUERYTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include <QMenu>
#include <QActionGroup>
#include "musicfillitemtablewidget.h"
#include "musicabstractqueryrequest.h"

/*! @brief The class of the query table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryTableWidget(QWidget *parent = nullptr);
    ~MusicQueryTableWidget();

    /*!
     * Set network query input.
     */
    virtual void setQueryInput(MusicAbstractQueryRequest *query);
    /*!
     * Get network query input.
     */
    MusicAbstractQueryRequest *queryInput() const;
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
     * Add current network music buffer to playlist.
     */
    void songBufferToPlaylist(const MusicResultDataItem &songItem);
    /*!
     * Restart search query by name.
     */
    void restartSearchQuery(const QString &name);

public Q_SLOTS:
    /*!
     * Table widget item cell double clicked.
     * Subclass should implement this function.
     */
    virtual void itemDoubleClicked(int row, int column) = 0;
    /*!
     * Create searched items.
     * Subclass should implement this function.
     */
    virtual void createSearchedItem(const MusicResultInfoItem &songItem) = 0;

protected:
    MusicAbstractQueryRequest *m_networkRequest;

};

#endif // MUSICQUERYTABLEWIDGET_H
