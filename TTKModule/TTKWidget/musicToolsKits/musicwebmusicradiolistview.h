#ifndef MUSICWEBMUSICRADIOLISTVIEW_H
#define MUSICWEBMUSICRADIOLISTVIEW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include <QListWidget>
#include "musicglobaldefine.h"

class QNetworkCookieJar;
class MusicRadioChannelThread;
class MusicWebMusicRadioWidget;

/*! @brief The class of the web music radio list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioListView : public QListWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMusicRadioListView(QWidget *parent = 0);

    ~MusicWebMusicRadioListView();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * To init list items.
     */
    void initListItems();

public Q_SLOTS:
    /*!
     * Radio list item has clicked.
     */
    void itemHasClicked(QListWidgetItem *item);
    /*!
     * Add radio list into list widget.
     */
    void addListWidgetItem();

protected:
    QNetworkCookieJar *m_cookJar;
    MusicRadioChannelThread *m_getChannelThread;
    MusicWebMusicRadioWidget *m_musicRadio;

};

#endif // MUSICWEBMUSICRADIOLISTVIEW_H
