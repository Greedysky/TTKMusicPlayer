#ifndef MUSICSONGCHECKTOOLSTABLEWIDGET_H
#define MUSICSONGCHECKTOOLSTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicsongchecktoolsunit.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song check tools rename table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsRenameTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsRenameTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsRenameTableWidget(QWidget *parent = nullptr);

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongCheckToolsRenameList &items);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

private:
    /*!
     * Remove current selected row.
     */
    void removeCurrentRow();

};



/*! @brief The class of the song check tools duplicate table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsDuplicateTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsDuplicateTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsDuplicateTableWidget(QWidget *parent = nullptr);
    ~MusicSongCheckToolsDuplicateTableWidget();

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongCheckToolsDuplicateList &songs);

Q_SIGNALS:
    /*!
     * Add current selected song to playlist.
     */
    void addSongToPlaylist(const QStringList &items);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

private:
    /*!
     * Add selected music song path to list.
     */
    void playSong();
    /*!
     * Remove current selected row.
     */
    void removeCurrentRow();

};



/*! @brief The class of the song check tools quality table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongCheckToolsQualityTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongCheckToolsQualityTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongCheckToolsQualityTableWidget(QWidget *parent = nullptr);
    ~MusicSongCheckToolsQualityTableWidget();

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongCheckToolsQualityList &songs);

Q_SIGNALS:
    /*!
     * Add current selected song to playlist.
     */
    void addSongToPlaylist(const QStringList &items);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

private:
    /*!
     * Add selected music song path to list.
     */
    void playSong();
    /*!
     * Remove current selected row.
     */
    void removeCurrentRow();

};

#endif // MUSICSONGCHECKTOOLSTABLEWIDGET_H
