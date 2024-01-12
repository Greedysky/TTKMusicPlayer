#ifndef MUSICLRCSEARCHTABLEWIDGET_H
#define MUSICLRCSEARCHTABLEWIDGET_H

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

#include "musicitemsearchtablewidget.h"

/*! @brief The class of the lrc search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcSearchTableWidget : public MusicItemSearchTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcSearchTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcSearchTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcSearchTableWidget();

    /*!
     * Start to search query by text.
     */
    virtual void startToSearchByText(const QString &text) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override final;

Q_SIGNALS:
    /*!
     * Lrc search state is finished.
     */
    void resolvedSuccess();
    /*!
     * Lrc download state is finished.
     */
    void lrcDownloadStateChanged(const QString &name);

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Table widget item cell double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Create search result items.
     */
    virtual void createResultItem(const MusicResultInfoItem &songItem) override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};

#endif // MUSICLRCSEARCHTABLEWIDGET_H
