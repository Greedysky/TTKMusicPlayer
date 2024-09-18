#ifndef MUSICCONNECTTRANSFERWIDGET_H
#define MUSICCONNECTTRANSFERWIDGET_H

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

#include "musicsong.h"
#include "musicsearchinterface.h"
#include "musicabstractmovedialog.h"

namespace Ui {
class MusicConnectTransferWidget;
}
struct MusicDeviceInfoItem;
class MusicConnectTransferThread;

/*! @brief The class of the transfer file to mobile widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConnectTransferWidget : public MusicAbstractMoveDialog, private MusicItemSearchInterface<MusicSongList>
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicConnectTransferWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicConnectTransferWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicConnectTransferWidget();

    /*!
     * Set current device info item.
     */
    void setDeviceInfoItem(MusicDeviceInfoItem *item);

public Q_SLOTS:
    /*!
     * Button state changed.
     */
    void itemSelectedChanged();
    /*!
     * Current playList selected.
     */
    void currentPlaylistSelected(int index);
    /*!
     * Select all items.
     */
    void selectedAllItems(bool checked);
    /*!
     * Start to transfer files.
     */
    void startToTransferFiles();
    /*!
     * Search result from list.
     */
    void searchResultChanged(int row, int column);
    /*!
     * Create the left button column
     */
    void initialize();

private:
    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongList &songs);
    /*!
     * Get selected files.
     */
    QStringList selectedFiles() const;

    Ui::MusicConnectTransferWidget *m_ui;

    int m_currentIndex;
    MusicDeviceInfoItem *m_currentDeviceItem;
    QString m_songCountLabel, m_selectCountLabel;
    MusicConnectTransferThread *m_thread;

};

#endif // MUSICCONNECTTRANSFERWIDGET_H
