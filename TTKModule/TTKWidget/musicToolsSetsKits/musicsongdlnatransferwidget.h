#ifndef MUSICSONGDLNATRANSFERWIDGET_H
#define MUSICSONGDLNATRANSFERWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongDlnaTransferWidget;
}

class QDlnaClient;
class QDlnaFinder;
class QDlnaFileServer;

/*! @brief The class of the song dlna transfer widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongDlnaTransferWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongDlnaTransferWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongDlnaTransferWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongDlnaTransferWidget();

public Q_SLOTS:
    /*!
     * Start to scan dlna devices.
     */
    void startToScan();
    /*!
     * Scan dlna devices finished.
     */
    void scanFinished();
    /*!
     * Set media to play.
     */
    void playSongAction();
    /*!
     * Set media to previous.
     */
    void playPrevious();
    /*!
     * Set media to next.
     */
    void playNext();

private Q_SLOTS:
    /*!
     * Player one second time out.
     */
    void timeout();

private:
    /*!
     * Get current selected client.
     */
    QDlnaClient *getClient() const;

private:
    Ui::MusicSongDlnaTransferWidget *m_ui;
    TTK::PlayState m_state;
    int m_currentPlayIndex;
    QTimer *m_timer;
    QDlnaFinder *m_dlnaFinder;
    QDlnaFileServer *m_dlnaFileServer;

};

#endif // MUSICSONGDLNATRANSFERWIDGET_H
