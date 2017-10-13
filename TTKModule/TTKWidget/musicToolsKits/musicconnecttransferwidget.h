#ifndef MUSICCONNECTTRANSFERWIDGET_H
#define MUSICCONNECTTRANSFERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include "musicsong.h"
#include "musicobject.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicConnectTransferWidget;
}
class QButtonGroup;
class MusicFileSenderServer;

/*! @brief The class of the transfer file to mobile widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectTransferWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConnectTransferWidget(QWidget *parent = 0);

    virtual ~MusicConnectTransferWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Open transfer files by mode.
     */
    void openTransferFiles(int mode);
    /*!
     * Redirect to current song.
     */
    void redirectToCurrentSong(int toolIndex, int songIndex);

Q_SIGNALS:
    /*!
     * Get music datas from container.
     */
    void getMusicLists(MusicSongItems &songs);

public Q_SLOTS:
    /*!
     * Current playList selected.
     */
    void currentPlayListSelected(int index);
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);
    /*!
     * Start to transfer usb files.
     */
    void startToTransferUSBFiles();
    /*!
     * Start to transfer wifi files.
     */
    void startToTransferWIFIFiles();
    /*!
     * Reflash removable dir.
     */
    void reflashRemovableDir();
    /*!
     * Switch to different device.
     */
    void switchDiffDevice();
    /*!
     * Search file from list.
     */
    void musicSearchIndexChanged(int row, int col);
    /*!
     * Create the left button column
     */
    void initColumns();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Create all items.
     */
    void createAllItems(const MusicSongs &songs);
    /*!
     * Get selected files.
     */
    QStringList getSelectedFiles();
    /*!
     * Get removable drive name.
     */
    QString getRemovableDrive();

    Ui::MusicConnectTransferWidget *m_ui;
    int m_currentIndex;
    MusicSongs m_currentSongs;
    QButtonGroup *m_buttonGroup;
    MusicObject::MIntsListMap m_searchfileListCache;
    MusicFileSenderServer *m_sendServer;

};

#endif // MUSICCONNECTTRANSFERWIDGET_H
