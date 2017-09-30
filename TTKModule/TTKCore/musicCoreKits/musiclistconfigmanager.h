#ifndef MUSICLISTCONFIGMANAGER_H
#define MUSICLISTCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

/*! @brief The class of the music list xml config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicListConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicListConfigManager(QObject *parent = nullptr);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readMusicXMLConfig() { return readConfig(MUSICPATH_FULL); }

    /*!
     * Write music datas into xml file.
     */
    void writeMusicSongsConfig(const MusicSongItems &musics);
    /*!
     * Write music datas into xml file.
     */
    void writeMusicSongsConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Read music datas into xml file.
     */
    void readMusicSongsConfig(MusicSongItems &musics);

protected:
    /*!
     * Read Music File Path.
     */
    MusicSongs readMusicFilePath(const QDomNode &node) const;

};

#endif // MUSICLISTCONFIGMANAGER_H
