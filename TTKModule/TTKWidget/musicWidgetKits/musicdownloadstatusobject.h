#ifndef MUSICDOWNLOADSTATUSOBJECT_H
#define MUSICDOWNLOADSTATUSOBJECT_H

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

#include "musicdownloadquerythreadabstract.h"

class MusicApplication;

/*! @brief The class of the show download state object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicDownloadStatusObject : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadStatusObject)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadStatusObject(QObject *parent);

    ~MusicDownloadStatusObject();

    /*!
     * Check the current song already has lrc or not,
     * if not just download it.
     */
    void musicCheckHasLrcAlready();
    /*!
     * Check the setting has open inline or desktop lrc on or not.
     */
    bool checkSettingParameterValue() const;

public Q_SLOTS:
    /*!
     * Download current music lrc from net.
     */
    void musicHaveNoLrcAlready();
    /*!
     * Show download state type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void showDownLoadInfoFor(MusicObject::DownLoadMode type);
    /*!
     * Show download state string type.
     * This is a slot by MusicSongSearchOnlineWidget's emit
     */
    void showDownLoadInfoFinished(const QString &type);
    /*!
     * Show current net connect state changed by net thread.
     */
    void networkConnectionStateChanged(bool state);

protected:
    bool m_previousState;
    MusicApplication *m_parentWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadLrcThread;

};

#endif // MUSICDOWNLOADSTATUSOBJECT_H
