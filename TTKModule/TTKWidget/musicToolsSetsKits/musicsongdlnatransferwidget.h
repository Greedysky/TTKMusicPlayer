#ifndef MUSICSONGDLNATRANSFERWIDGET_H
#define MUSICSONGDLNATRANSFERWIDGET_H

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

namespace Ui {
class MusicSongDlnaTransferWidget;
}

class DlnaFinder;

/*! @brief The class of the song dlna transfer widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongDlnaTransferWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongDlnaTransferWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongDlnaTransferWidget(QWidget *parent = nullptr);

    virtual ~MusicSongDlnaTransferWidget();

Q_SIGNALS:

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
     * Override exec function.
     */
    virtual int exec();

private:
    Ui::MusicSongDlnaTransferWidget *m_ui;

    DlnaFinder *m_dlnaFinder;

};

#endif // MUSICSONGDLNATRANSFERWIDGET_H
