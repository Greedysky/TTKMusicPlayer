#ifndef MUSICLRCLOCALLINKWIDGET_H
#define MUSICLRCLOCALLINKWIDGET_H

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

#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the local data item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_LRC_EXPORT MusicLocalDataItem
{
    QString m_name;
    QString m_path;
}MusicLocalDataItem;
TTK_DECLARE_LISTS(MusicLocalDataItem)

/*! @brief The class of the local lrc link table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcLocalLinkTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcLocalLinkTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcLocalLinkTableWidget(QWidget *parent = nullptr);

    /*!
     * Check if current item has in.
     */
    bool contains(const QString &string);
    /*!
     * Create all items.
     */
    void createAllItems(const MusicLocalDataItems &items);

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;

};


namespace Ui {
class MusicLrcLocalLinkWidget;
}

/*! @brief The class of the local lrc link widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcLocalLinkWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcLocalLinkWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcLocalLinkWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcLocalLinkWidget();

    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

Q_SIGNALS:
    /*!
     * Current lrc changed by user, and emit Lrc flag.
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
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Search local lrc in MLrc dir.
     */
    void searchInLocalMLrc();

    QString m_currentName;
    Ui::MusicLrcLocalLinkWidget *m_ui;

};

#endif // MUSICLRCLOCALLINKWIDGET_H
