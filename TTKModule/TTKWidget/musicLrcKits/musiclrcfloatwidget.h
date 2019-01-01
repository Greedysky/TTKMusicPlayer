#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

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

#include "musicfloatabstractwidget.h"

class QPushButton;
class MusicLrcFloatPhotoWidget;
class MusicLrcFloatSettingWidget;

/*! @brief The class of the lrc float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcFloatWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcFloatWidget();

    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

public Q_SLOTS:
    /*!
     * Show float setting widget.
     */
    void showFloatSettingWidget();
    /*!
     * Close float setting widget.
     */
    void closeFloatSettingWidget();
    /*!
     * Lrc desktop wallpaper button clicked.
     */
    void musicContainerForWallpaperClicked();

protected:
    QPushButton *m_more, *m_update, *m_search;
    QPushButton *m_wallp, *m_photo;
    MusicLrcFloatPhotoWidget *m_floatPhotoWidget;
    MusicLrcFloatSettingWidget *m_floatSettingWidget;

};

#endif // MUSICLRCFLOATWIDGET_H
