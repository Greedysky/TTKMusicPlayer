#ifndef MUSICSONGLISTSHARINGWIDGET_H
#define MUSICSONGLISTSHARINGWIDGET_H

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
class MusicSongListSharingWidget;
}

/*! @brief The class of the song list share widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongListSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongListSharingWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongListSharingWidget(QWidget *parent = nullptr);

    virtual ~MusicSongListSharingWidget();

public Q_SLOTS:
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * Switch to diff widget.
     */
    void switchToDiffWidget(int index);
    /*!
     * Write copy button clicked.
     */
    void writeMainCopyButtonClicked();
    /*!
     * Read button clicked.
     */
    void readMainButtonClicked();

protected:
    /*!
     * Create write key.
     */
    void createWriteKey();

    Ui::MusicSongListSharingWidget *m_ui;

};

#endif // MUSICSONGLISTSHARINGWIDGET_H
