#ifndef MUSICLOCALSONGSTABLEWIDGET_H
#define MUSICLOCALSONGSTABLEWIDGET_H

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

#include "musicsongslistabstracttablewidget.h"

/*! @brief The class of the lcal songs table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongsTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsTableWidget(QWidget *parent = nullptr);

    virtual ~MusicLocalSongsTableWidget();

    /*!
     * Clear current items.
     */
    void clear();
    /*!
     * Add show list items.
     */
    void addItems(const QFileInfoList &path);
    /*!
     * Set files container.
     */
    inline void setFiles(const QFileInfoList &path) { m_fileNames = path; }
    /*!
     * Get files container.
     */
    inline const QFileInfoList &getFiles() const { return m_fileNames; }

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

protected:
    QFileInfoList m_fileNames;

};

typedef QMap<QString , QFileInfoList> MusicInfoData;
typedef QMapIterator<QString , QFileInfoList> MusicInfoDataIterator;

/*! @brief The class of the lcal songs info table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsInfoTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongsInfoTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsInfoTableWidget(QWidget *parent = nullptr);

    /*!
     * Clear current items.
     */
    void clear();
    /*!
     * Add show list item.
     */
    void addItems(const MusicInfoData &data);

Q_SIGNALS:
    /*!
     * Update file lists.
     */
    void updateFileLists(const QFileInfoList &list);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list double cell click.
     */
    void listCellDoubleClicked(int row, int column);

};

#endif // MUSICLOCALSONGSTABLEWIDGET_H
