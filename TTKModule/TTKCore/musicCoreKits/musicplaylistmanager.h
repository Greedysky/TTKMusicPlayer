#ifndef MUSICPLAYLISTMANAGER_H
#define MUSICPLAYLISTMANAGER_H

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

#include "musicsong.h"
#include "musicabstractxml.h"

/*! @brief The class of the wpl XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicWPLConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWPLConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWPLConfigManager(QObject *parent = 0);

    /*!
     * Write datas into xml file.
     */
    void writeWPLXMLConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Read datas into xml file.
     */
    void readWPLXMLConfig(MusicSongItems &musics);

protected:
    /*!
     * Read Music File Path.
     */
    MusicSongs readMusicFilePath(const QDomNode &node) const;

};


/*! @brief The class of the xspf XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicXSPFConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicXSPFConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicXSPFConfigManager(QObject *parent = 0);

    /*!
     * Write datas into xml file.
     */
    void writeXSPFXMLConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Read datas into xml file.
     */
    void readXSPFXMLConfig(MusicSongItems &musics);

protected:
    /*!
     * Read Music File Path.
     */
    MusicSongs readMusicFilePath(const QDomNode &node) const;

};


/*! @brief The class of the asx XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicASXConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicASXConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicASXConfigManager(QObject *parent = 0);

    /*!
     * Write datas into xml file.
     */
    void writeASXXMLConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Read datas into xml file.
     */
    void readASXXMLConfig(MusicSongItems &musics);

};


/*! @brief The class of the kuwo XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicKWLConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWLConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicKWLConfigManager(QObject *parent = 0);

    /*!
     * Read datas from xml file by given name.
     */
    bool readConfig(const QString &name);
    /*!
     * Read datas into xml file.
     */
    void readKWLXMLConfig(MusicSongItems &musics);

};


/*! @brief The class of the kugou XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicKGLConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKGLConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicKGLConfigManager(QObject *parent = 0);

    /*!
     * Read datas into xml file.
     */
    void readKGLXMLConfig(MusicSongItems &musics);

};


/*! @brief The class of the playlist manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlayListManager
{
    TTK_DECLARE_MODULE(MusicPlayListManager)
public:
    /*!
     * Import error message alert.
     */
    static void messageAlert();
    /*!
     * Write music playlist data to file.
     */
    void setMusicSongItem(const QString &save, const MusicSongItem &item);
    /*!
     * Read music playlist data from file.
     */
    void getMusicSongItems(const QStringList &open, MusicSongItems &items);

protected:
    /*!
     * Read ttk music playlist data from file.
     */
    void readLisList(const QString &path, MusicSongItems &items);
    /*!
     * Write ttk music playlist data to file.
     */
    void writeLisList(const QString &path, const MusicSongItem &item);
    /*!
     * Read m3u music playlist data from file.
     */
    void readM3UList(const QString &path, MusicSongItems &items);
    /*!
     * Write m3u music playlist data to file.
     */
    void writeM3UList(const QString &path, const MusicSongItem &item);
    /*!
     * Read pls music playlist data from file.
     */
    void readPLSList(const QString &path, MusicSongItems &items);
    /*!
     * Write pls music playlist data to file.
     */
    void writePLSList(const QString &path, const MusicSongItem &item);
    /*!
     * Read wpl music playlist data from file.
     */
    void readWPLList(const QString &path, MusicSongItems &items);
    /*!
     * Write wpl music playlist data to file.
     */
    void writeWPLList(const QString &path, const MusicSongItem &item);
    /*!
     * Read xspf music playlist data from file.
     */
    void readXSPFList(const QString &path, MusicSongItems &items);
    /*!
     * Write xspf music playlist data to file.
     */
    void writeXSPFList(const QString &path, const MusicSongItem &item);
    /*!
     * Read asx music playlist data from file.
     */
    void readASXList(const QString &path, MusicSongItems &items);
    /*!
     * Write asx music playlist data to file.
     */
    void writeASXList(const QString &path, const MusicSongItem &item);
    /*!
     * Read kuwo music playlist data from file.
     */
    void readKWLList(const QString &path, MusicSongItems &items);
    /*!
     * Read kugou music playlist data from file.
     */
    void readKGLList(const QString &path, MusicSongItems &items);

};

#endif // MUSICPLAYLISTMANAGER_H
