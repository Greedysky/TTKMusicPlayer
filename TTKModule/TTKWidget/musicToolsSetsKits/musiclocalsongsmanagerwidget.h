#ifndef MUSICLOCALSONGSMANAGERWIDGET_H
#define MUSICLOCALSONGSMANAGERWIDGET_H

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

#include <QFileInfo>
#include <QMouseEvent>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicwidgetheaders.h"
#include "musicabstractmovewidget.h"

class MusicLocalSongsManagerThread;

namespace Ui {
class MusicLocalSongsManagerWidget;
}

/*! @brief The class of the lcal songs manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicLocalSongsManagerWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongsManagerWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongsManagerWidget(QWidget *parent = nullptr);

    virtual ~MusicLocalSongsManagerWidget();

    /*!
     * Find extra device path.
     */
    void findExtraDevicePath(const QString &dir);

Q_SIGNALS:
    /*!
     * Add current selected song to play lists.
     */
    void addSongToPlay(const QStringList &names);

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);
    /*!
     * Music song audition play.
     */
    void auditionButtonClick();
    /*!
     * Add music song to play list.
     */
    void addButtonClick();
    /*!
     * Item cell on click by row and col.
     */
    void itemCellOnClick(int row, int col);
    /*!
     * Item cell on double click by row and col.
     */
    void itemDoubleClicked(int row, int col);
    /*!
     * Send the searched file or path.
     */
    void setSongNamePath(const QFileInfoList &name);
    /*!
     * Start to fetch file or files.
     */
    void filterScanChanged(int index);
    /*!
     * Search file from list.
     */
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Update file lists.
     */
    void updateFileLists(const QFileInfoList &list);

    /*!
     * Select to show list mode.
     */
    void setShowlistButton();
    /*!
     * Select to show art mode.
     */
    void setShowArtButton();
    /*!
     * Select to show album mode.
     */
    void setShowAlbumButton();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Clear All Items.
     */
    void clearAllItems();
    /*!
     * Add all file items into list.
     */
    void addAllItems(const QFileInfoList &name);
    /*!
     * Add current system all supported drives list.
     */
    void addDrivesList();
    /*!
     * Set current item or items selected.
     */
    void itemsSelected();
    /*!
     * Current drive index changed.
     */
    bool filterIndexChanged();
    /*!
     * Current custom dir path changed.
     */
    bool filterIndexCustChanged();
    /*!
     * Control enable or disable.
     */
    void controlEnable(bool state);
    /*!
     * Loading label disable.
     */
    void loadingLabelState(bool state);

    bool m_runTypeChanged;
    Ui::MusicLocalSongsManagerWidget *m_ui;
    QFileInfoList m_fileNames;
    MusicLocalSongsManagerThread *m_thread;
    MIntListMap m_searchfileListCache;

};

#endif // MUSICLOCALSONGSMANAGERWIDGET_H
