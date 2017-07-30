#ifndef MUSICPLAYLISTMANAGER_H
#define MUSICPLAYLISTMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicabstractxml.h"

/*! @brief The class of the wpl XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicWPLConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicWPLConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void writeMusicSongsConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Write music datas into xml file.
     */
    void readMusicSongsConfig(MusicSongItems &musics);
    /*!
     * Read music datas into xml file.
     */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};


/*! @brief The class of the xspf XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicXSPFConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicXSPFConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void writeMusicSongsConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Write music datas into xml file.
     */
    void readMusicSongsConfig(MusicSongItems &musics);
    /*!
     * Read music datas into xml file.
     */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};


/*! @brief The class of the playlist manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlayListManager
{
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */
    static void messageAlert();
    /*!
     * Import error message alert.
     */
    void setMusicSongItems(const QString &save, const MusicSongItem &item);
    /*!
     * Write music playlist data to file.
     */
    void getMusicSongItems(const QStringList &open, MusicSongItems &items);
    /*!
     * Read music playlist data from file.
     */

protected:
    void readLisList(const QString &path, MusicSongItems &items);
    /*!
     * Read ttk music playlist data from file.
     */
    void writeLisList(const QString &path, const MusicSongItem &item);
    /*!
     * Write ttk music playlist data to file.
     */
    void readM3UList(const QString &path, MusicSongItems &items);
    /*!
     * Read m3u music playlist data from file.
     */
    void writeM3UList(const QString &path, const MusicSongItem &item);
    /*!
     * Write m3u music playlist data to file.
     */
    void readPLSList(const QString &path, MusicSongItems &items);
    /*!
     * Read pls music playlist data from file.
     */
    void writePLSList(const QString &path, const MusicSongItem &item);
    /*!
     * Write pls music playlist data to file.
     */
    void readWPLList(const QString &path, MusicSongItems &items);
    /*!
     * Read wpl music playlist data from file.
     */
    void writeWPLList(const QString &path, const MusicSongItem &item);
    /*!
     * Write wpl music playlist data to file.
     */
    void readXSPFList(const QString &path, MusicSongItems &items);
    /*!
     * Read xspf music playlist data from file.
     */
    void writeXSPFList(const QString &path, const MusicSongItem &item);
    /*!
     * Write xspf music playlist data to file.
     */

};

#endif // MUSICPLAYLISTMANAGER_H
