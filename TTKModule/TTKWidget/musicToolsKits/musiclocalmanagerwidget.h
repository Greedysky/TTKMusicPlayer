#ifndef MUSICLOCALMANAGERWIDGET_H
#define MUSICLOCALMANAGERWIDGET_H

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

#include "musicabstractsongslisttablewidget.h"

/*! @brief The class of the local song information.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSongInfoItem
{
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_year;
    QString m_genre;
    QString m_path;
};
TTK_DECLARE_LIST(MusicSongInfoItem);

/*! @brief The class of the local manager songs table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLocalManagerSongsTableWidget : public MusicAbstractSongsListTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalManagerSongsTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalManagerSongsTableWidget(QWidget *parent = nullptr);
    ~MusicLocalManagerSongsTableWidget();

    /*!
     * Update songs files in table.
     */
    void updateSongsList(const QStringList &songs);

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
class MusicItemQueryEdit;
class MusicGifLabelWidget;

/*! @brief The class of the local manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLocalManagerWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalManagerWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalManagerWidget(QWidget *parent = nullptr);
    ~MusicLocalManagerWidget();

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

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

    QLabel *m_sizeLabel;
    QTabWidget *m_tabWidget;
    MusicItemQueryEdit *m_searchEdit;
    MusicGifLabelWidget *m_loadingLabel;
    TTKIntListMap m_searchResultCache;
    MusicSongInfoItemList m_songItems;

};

#endif // MUSICLOCALMANAGERWIDGET_H
