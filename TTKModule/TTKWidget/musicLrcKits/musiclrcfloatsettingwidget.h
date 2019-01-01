#ifndef MUSICLRCFLOATSETTINGWIDGET_H
#define MUSICLRCFLOATSETTINGWIDGET_H

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

/*! @brief The class of the lrc setting float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFloatSettingWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcFloatSettingWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcFloatSettingWidget(QWidget *parent = nullptr);

    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

Q_SIGNALS:
    /*!
     * Widget close it emit.
     */
    void widgetClose();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Change current lrc size up.
     */
    void lrcSizeUpChanged();
    /*!
     * Change current lrc size down.
     */
    void lrcSizeLowChanged();
    /*!
     * Change current background to default.
     */
    void lrcMusicBackgroundChanged();
    /*!
     * Change current background to artist.
     */
    void lrcArtBackgroundChanged();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Create pushButton by given index.
     */
    QPushButton *createPushButton(int index);

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
