#ifndef MUSICSONGDLNATRANSFERWIDGET_H
#define MUSICSONGDLNATRANSFERWIDGET_H

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
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongDlnaTransferWidget;
}

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

Q_SIGNALS:
    /*!
     * Query music datas from container.
     */
    void queryMusicItemList(MusicSongItemList &songs);

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
     * Set current player position.
     */
    void positionChanged(qint64 position);
    /*!
     * Set current player duration.
     */
    void durationChanged(qint64 duration);
    /*!
     * Set music to play.
     */
    void playSongClicked();
    /*!
     * Set music to previous.
     */
    void playPrevious();
    /*!
     * Set music to next.
     */
    void playNext();

private:
    Ui::MusicSongDlnaTransferWidget *m_ui;
    bool m_isPlaying;
    int m_currentPlayIndex;
    MusicSongList *m_songs;
    QDlnaFinder *m_dlnaFinder;
    QDlnaFileServer *m_dlnaFileServer;

};

#endif // MUSICSONGDLNATRANSFERWIDGET_H
