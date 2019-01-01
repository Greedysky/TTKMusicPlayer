#ifndef MUSICLRCMANAGERFORINLINE_H
#define MUSICLRCMANAGERFORINLINE_H

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

#include "musiclrcmanager.h"

#define LRC_PER_WIDTH 670

/*! @brief The class of the inline lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcManagerForInline : public MusicLrcManager
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcManagerForInline)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcManagerForInline(QWidget *parent = nullptr);

    /*!
     * Set lrc per width.
     */
    inline void setLrcPerWidth(int width) { m_lrcPerWidth = width + LRC_PER_WIDTH;}
    /*!
     * Set adjust font size.
     */
    inline void setFontSize(int size) { m_gradientFontSize = size;}
    /*!
     * Set adjust transparent by value.
     */
    inline void setTransparent(int tran) { m_gradientTransparent = tran;}

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    int m_gradientFontSize;
    int m_gradientTransparent;

};

#endif // MUSICLRCMANAGERFORINLINE_H
