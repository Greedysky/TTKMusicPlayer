#ifndef MUSICAPPLICATION_H
#define MUSICAPPLICATION_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QObject>
#include "musicmobileglobaldefine.h"

class QQmlContext;
class TTKMusicUtils;
class TTKNetworkHelper;

/*! @brief The class of the music application.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT MusicApplication : public QObject
{
    Q_OBJECT
public:
    explicit MusicApplication(QQmlContext *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicApplication();

    Q_INVOKABLE bool checkMusicListCurrentIndex() const;
    /*!
     * Check current list index is -1 or not.
     */
    Q_INVOKABLE QString getCurrentFileName() const;
    /*!
     * Get current file name.
     */
    Q_INVOKABLE void musicLoadCurrentSongLrc();

Q_SIGNALS:
    void updateCurrentArtist();
    /*!
     * Download current artist image finished emit.
     */

protected:
    void readXMLConfigFromText();
    /*!
     * Read XML config from text.
     */
    void writeXMLConfigToText();
    /*!
     * Write XML config to text.
     */

    TTKMusicUtils *m_ttkUtils;
    TTKNetworkHelper *m_networkHelper;

};

#endif // MUSICAPPLICATION_H
