#ifndef TTKMUSICCONFIGMANAGER_H
#define TTKMUSICCONFIGMANAGER_H

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

    inline int readShowLrcColor() const
    { return readXmlAttributeByTagNameValue("lrcColor").toInt(); }
    /*!
     * Read Show Lrc Color Config.
     */
    inline int readShowLrcSize() const
    { return readXmlAttributeByTagNameValue("lrcSize").toInt(); }
    /*!
     * Read Show Lrc Size Config.
     */
    inline int readShowLrcType() const
    { return readXmlAttributeByTagNameValue("lrcType").toInt(); }
    /*!
     * Read Show Lrc Type Config.
     */
    inline QString readShowLrcFgColor() const
    { return readXmlAttributeByTagNameValue("lrcFgColor");}
    /*!
     * Read Show Lrc Fg Color Config.
     */

    bool readNeedUpdateConfig();
    /*!
     * Read need update config.
     */
    void readSystemLastPlayIndexConfig(QStringList &key) const;
   /*!
    * Read System Last Play Index Config.
    */
    void readOtherLoadConfig() const;
    /*!
     * Read Other Load Config.
     */

protected:
    MusicSongs readMusicFilePath(const QDomNode &node) const;
    /*!
     * Read Music File Path.
     */

};

#endif // TTKMUSICCONFIGMANAGER_H
