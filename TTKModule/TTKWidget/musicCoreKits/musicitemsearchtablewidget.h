#ifndef MUSICITEMSEARCHTABLEWIDGET_H
#define MUSICITEMSEARCHTABLEWIDGET_H

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

#include "musicquerytablewidget.h"

class MusicLabelDelegate;
class MusicGifLabelWidget;

/*! @brief The class of the search item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemSearchTableWidget : public MusicQueryTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicItemSearchTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicItemSearchTableWidget(QWidget *parent = nullptr);
    ~MusicItemSearchTableWidget();

    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
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

#endif // MUSICITEMSEARCHTABLEWIDGET_H
