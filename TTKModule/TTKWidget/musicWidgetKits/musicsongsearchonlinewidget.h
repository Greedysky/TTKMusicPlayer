#ifndef MUSICSONGSEARCHONLINEWIDGET_H
#define MUSICSONGSEARCHONLINEWIDGET_H

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

#include "musicabstractqueryrequest.h"
#include "musicitemsearchtablewidget.h"

class QLabel;
class QPushButton;
class MusicCoreMPlayer;

/*! @brief The class of the song search online table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSearchTableWidget : public MusicItemSearchTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSearchTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongSearchTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSearchTableWidget();

    /*!
     * Start to search query by text.
     */
    virtual void startToSearchByText(const QString &text) override final;
    /*!
     * Download query result data.
     */
    virtual void downloadQueryResult(int row) override final;

    /*!
     * Start to search query by given id.
     */
    void startToSearchByID(const QString &text);
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection();

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
    /*!
     * Search action menu clicked.
     */
    virtual void searchActionClicked(QAction *action) override final;

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    /*!
     * Add search music to play list by index.
     */
    void addSearchMusicToPlaylist(int row, bool play);

    friend class MusicSongSearchOnlineWidget;
};



/*! @brief The class of the song search online widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSearchOnlineWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSearchOnlineWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongSearchOnlineWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSearchOnlineWidget();

    /*!
     * Start to search query by text.
     */
    void startToSearchByText(const QString &name);
    /*!
     * Start to search query by given id.
     */
    void startToSearchByID(const QString &name);
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * Set diff button clicked by index.
     */
    void buttonClicked(int index);

private:
    /*!
     * Create tool widget.
     */
    void createToolWidget(QWidget *widget);
    /*!
     * Set resize labelt ext.
     */
    void setResizeLabelText(const QString &name);

    QLabel *m_textLabel;
    QPushButton *m_playButton;
    QWidgetList m_resizeWidgets;
    MusicSongSearchTableWidget *m_searchTableWidget;

};

#endif // MUSICSONGSEARCHONLINEWIDGET_H
