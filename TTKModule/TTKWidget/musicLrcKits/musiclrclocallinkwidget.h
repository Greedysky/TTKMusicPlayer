#ifndef MUSICLRCLOCALLINKWIDGET_H
#define MUSICLRCLOCALLINKWIDGET_H

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

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the local data item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicLocalDataItem
{
    QString m_name;
    QString m_path;
};
TTK_DECLARE_LIST(MusicLocalDataItem);

/*! @brief The class of the local lrc link table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcLocalLinkTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcLocalLinkTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcLocalLinkTableWidget(QWidget *parent = nullptr);

    /*!
     * Check if current item has in.
     */
    bool contains(const QString &string);
    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicLocalDataItemList &items);

};


namespace Ui {
class MusicLrcLocalLinkWidget;
}

/*! @brief The class of the local lrc link widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcLocalLinkWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcLocalLinkWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcLocalLinkWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcLocalLinkWidget();

    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

Q_SIGNALS:
    /*!
     * Current lrc changed by user.
     */
    void currentLrcChanged(const QString &data);

public Q_SLOTS:
    /*!
     * Search fuzzy state changed.
     */
    void fuzzyStateChanged();
    /*!
     * Find in local file button clicked.
     */
    void findInLocalFile();
    /*!
     * Delete found lrc button clicked.
     */
    void deleteFoundLrc();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();

private:
    /*!
     * Search local lrc in dir.
     */
    void searchInLocalLrc();

    Ui::MusicLrcLocalLinkWidget *m_ui;
    QString m_currentName;

};

#endif // MUSICLRCLOCALLINKWIDGET_H
