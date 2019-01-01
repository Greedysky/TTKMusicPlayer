#ifndef MUSICWEBRADIOVIEW_H
#define MUSICWEBRADIOVIEW_H

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

#include <QWidget>
#include "musicglobaldefine.h"

class MusicWebMusicRadioWidget;

/*! @brief The class of the web radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebRadioView : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebRadioView)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebRadioView(QWidget *parent = nullptr);

    ~MusicWebRadioView();

    /*!
     * To init list items.
     */
    void init(int index);

public Q_SLOTS:
    /*!
     * Open Dj Radio window has clicked.
     */
    void openDJRadioWindow();
    /*!
     * Open mv Radio window has clicked.
     */
    void openMVRadioWindow();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    MusicWebMusicRadioWidget *m_musicRadio;

};

#endif // MUSICWEBRADIOVIEW_H
