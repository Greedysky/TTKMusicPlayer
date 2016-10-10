#ifndef TTKMUSICCONFIGMANAGER_H
#define TTKMUSICCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"
#include "musicmobileglobaldefine.h"

/*! @brief The class of the config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit TTKMusicConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    inline bool readXMLConfig(const QString &path) { return readConfig(path); }
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readMusicXMLConfig(const QString &path){ return readConfig(path); }
    /*!
     * Read music datas from xml file by given name.
     */
    void writeXMLConfig();
    /*!
     * Write datas into xml file.
     */
    void writeMusicSongsConfig(const MusicSongItems &musics, const QString &path);
    /*!
     * Write music datas into xml file.
     */

    void readMusicSongsConfig(MusicSongItems &musics);
    /*!
     * Read music datas into xml file.
     */
    inline int readMusicPlayVolumeConfig() const
    { return readXmlAttributeByTagNameValue("playVolume").toInt(); }
    /*!
     * Read Music Play Volume Config.
     */
    inline int readMusicPlayModeConfig() const
    { return readXmlAttributeByTagNameValue("playMode").toInt(); }
    /*!
     * Read Music Play Mode Config.
     */
    inline int readEnhancedMusicConfig() const
    { return readXmlAttributeByTagNameValue("enhancedMusic").toInt(); }
    /*!
     * Read Enhanced Music Config.
     */
    inline int readEqualizerIndex() const
    { return readXmlAttributeByTagNameValue("equalizerIndex").toInt(); }
    /*!
     * Read Equalizer Index Config.
     */
    inline int readEqualizerEnable() const
    { return readXmlAttributeByTagNameValue("equalizerEnable").toInt(); }
    /*!
     * Read Equalizer Enale Config.
     */
    inline QString readEqualizerValue() const
    { return readXmlAttributeByTagNameValue("equalizerValue"); }
    /*!
     * Read Equalizer Value Config.
     */

    void readSystemLastPlayIndexConfig(QStringList &key) const;
   /*!
    * Read System Last Play Index Config.
    */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};

#endif // TTKMUSICCONFIGMANAGER_H
