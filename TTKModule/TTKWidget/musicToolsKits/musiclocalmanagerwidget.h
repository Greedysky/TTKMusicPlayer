#ifndef MUSICLOCALMANAGERWIDGET_H
#define MUSICLOCALMANAGERWIDGET_H

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

#include "musicsearchinterface.h"
#include "musicabstractsongslisttablewidget.h"

/*! @brief The class of the local song item information.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongInfoItem
{
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_year;
    QString m_genre;
    QString m_track;
    QString m_path;
};
TTK_DECLARE_LIST(MusicSongInfoItem);

using MusicSongStatisticItem = QMap<QString, int>;

/*! @brief The class of the local manager statistic table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLocalManagerStatisticTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalManagerStatisticTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLocalManagerStatisticTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLocalManagerStatisticTableWidget();

    /*!
     * Create cell items by input data.
     */
    void addCellItem(const MusicSongStatisticItem &song);
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

};


/*! @brief The class of the local manager songs table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLocalManagerSongsTableWidget : public MusicAbstractSongsListTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalManagerSongsTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLocalManagerSongsTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLocalManagerSongsTableWidget();

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongInfoItemList &songs);
    /*!
     * Resize section bound by resize called.
     */
    void resizeSection() const;

public Q_SLOTS:
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};

class QLabel;
class TTKTabButton;
class MusicItemQueryEdit;
class MusicGifLabelValueWidget;

/*! @brief The class of the local manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLocalManagerWidget : public QWidget, public TTKAbstractResizeInterface, private MusicItemSearchInterface<MusicSongInfoItemList>
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalManagerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLocalManagerWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLocalManagerWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

private Q_SLOTS:
    /*!
     * Type index changed.
     */
    void typeIndexChanged(int index);
    /*!
     * Refresh table items.
     */
    void refreshItems();
    /*!
     * Update media library path.
     */
    void updateMediaLibraryPath();
    /*!
     * Search result from list.
     */
    void searchResultChanged(int row, int column);
    /*!
     * Item cell on double click by row and col.
     */
    void itemDoubleClicked(int row, int column);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    /*!
     * Update statistic widget.
     */
    void updateStatisticWidget(int index, const MusicSongInfoItemList &items);

    QLabel *m_sizeLabel;
    TTKTabButton *m_tabButton;
    MusicItemQueryEdit *m_searchEdit;
    MusicGifLabelValueWidget *m_loadingLabel;
    MusicLocalManagerSongsTableWidget *m_songTableWidget;
    MusicLocalManagerStatisticTableWidget *m_statisticTableWidget;

};

#endif // MUSICLOCALMANAGERWIDGET_H
