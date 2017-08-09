#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QColor>
#include "musicabstractxml.h"

/*! @brief The class of the XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicXMLConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicXMLConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readXMLConfig() { return readConfig(COFIGPATH_FULL); }
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readMusicXMLConfig() { return readConfig(MUSICPATH_FULL); }
    /*!
     * Read music datas from xml file by given name.
     */

    void writeXMLConfig();
    /*!
     * Write datas into xml file.
     */
    void writeMusicSongsConfig(const MusicSongItems &musics,
                               const QString &path = MUSICPATH_FULL);
    /*!
     * Write music datas into xml file.
     */

    void readMusicSongsConfig(MusicSongItems &musics);
    /*!
     * Read music datas into xml file.
     */

    void readSystemLastPlayIndexConfig(QStringList &key) const;
    /*!
     * Read System Last Play Index Config.
     */
    QRect readWindowGeometry() const;
    /*!
     * Read window widget Geometry Config.
     */
    QPoint readShowDLrcGeometry() const;
    /*!
     * Read Show Desktop Lrc Geometry Config.
     */
    void readSysLoadConfig() const;
    /*!
     * Read Sys Load Config.
     */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};

#endif // MUSICXMLCONFIGMANAGER_H
