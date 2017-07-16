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

    inline int readMusicPlayModeConfig() const
    { return readXmlAttributeByTagNameValue("playMode").toInt(); }
    /*!
     * Read Music Play Mode Config.
     */
    inline int readMusicPlayVolumeConfig() const
    { return readXmlAttributeByTagNameValue("playVolume").toInt(); }
    /*!
     * Read Music Play Volume Config.
     */
    inline int readSystemCloseConfig() const
    { return readXmlAttributeByTagNameValue("closeEvent").toInt(); }
    /*!
     * Read System Close Config.
     */
    inline int readCloseNetworkConfig() const
    { return readXmlAttributeByTagNameValue("closeNetwork").toInt(); }
    /*!
     * Read Close Network Config.
     */
    inline int readWindowConciseConfig() const
    { return readXmlAttributeByTagNameValue("windowConcise").toInt(); }
    /*!
     * Read Window Concise Config.
     */
    inline int readRemoteWidgetModeConfig() const
    { return readXmlAttributeByTagNameValue("remoteWidgetMode").toInt(); }
    /*!
     * Read Remote Widget Mode Config.
     */
    inline int readSystemAutoPlayConfig() const
    { return readXmlAttributeByTagNameValue("autoPlay").toInt(); }
    /*!
     * Read System Auto Play Config.
     */

    inline QString readOtherBgLossless() const
    { return readXmlAttributeByTagNameValue("otherBgLossless"); }
    /*!
     * Read Other Background Lossless Config.
     */
    inline QString readOtherUpdate() const
    { return readXmlAttributeByTagNameValue("otherUpdate"); }
    /*!
     * Read Other Update Check Config.
     */
    inline QString readOtherSearch() const
    { return readXmlAttributeByTagNameValue("otherSearch"); }
    /*!
     * Read Search Optimzied Config.
     */
    inline QString readOtherAlbum() const
    { return readXmlAttributeByTagNameValue("otherAlbum"); }
    /*!
     * Read Use File Album Config.
     */
    inline QString readOtherInfo() const
    { return readXmlAttributeByTagNameValue("otherInfo"); }
    /*!
     * Read Other Use File Info Config.
     */

    inline QString readBackgroundTheme() const
    { return readXmlAttributeByTagNameValue("bgTheme"); }
    /*!
     * Read Background Theme Config.
     */
    inline QString readBackgroundTransparent() const
    { return readXmlAttributeByTagNameValue("bgTransparent"); }
    /*!
     * Read Background Transparent Config.
     */
    inline QString readBackgroundListTransparent() const
    { return readXmlAttributeByTagNameValue("bgListTransparent"); }
    /*!
     * Read Background List Transparent Config.
     */

    inline int readShowInlineLrc() const
    { return readXmlAttributeByTagNameValue("showInlineLrc").toInt(); }
    /*!
     * Read Show Inline Lrc Config.
     */
    inline int readShowDesktopLrc() const
    { return readXmlAttributeByTagNameValue("showDesktopLrc").toInt(); }
    /*!
     * Read Music Show Desktop Lrc Config.
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
    inline int readShowLrcFamily() const
    { return readXmlAttributeByTagNameValue("lrcFamily").toInt(); }
    /*!
     * Read Show Lrc Family Config.
     */
    inline int readShowLrcType() const
    { return readXmlAttributeByTagNameValue("lrcType").toInt(); }
    /*!
     * Read Show Lrc Type Config.
     */
    inline int readShowLrcTransparent() const
    { return readXmlAttributeByTagNameValue("lrcTransparent").toInt(); }
    /*!
     * Read Show Lrc Transparent Config.
     */
    inline int readShowDLrcColor() const
    { return readXmlAttributeByTagNameValue("lrcDColor").toInt(); }
    /*!
     * Read Show Desktop Lrc Color Config.
     */
    inline int readShowDLrcSize() const
    { return readXmlAttributeByTagNameValue("lrcDSize").toInt(); }
    /*!
     * Read Show Desktop Lrc Size Config.
     */
    inline int readShowDLrcFamily() const
    { return readXmlAttributeByTagNameValue("lrcDFamily").toInt(); }
    /*!
     * Read Show Desktop Lrc Family Config.
     */
    inline int readShowDLrcType() const
    { return readXmlAttributeByTagNameValue("lrcDType").toInt(); }
    /*!
     * Read Show Desktop Lrc Type Config.
     */
    inline int readShowDLrcTransparent() const
    { return readXmlAttributeByTagNameValue("lrcDTransparent").toInt(); }
    /*!
     * Read Show Desktop Lrc Family Transparent Config.
     */
    inline int readDLrcWindowType() const
    { return readXmlAttributeByTagNameValue("lrcDWindowType").toInt(); }
    /*!
     * Read Show Desktop Lrc Window Type.
     */
    inline int readDLrcSingleLineType() const
    { return readXmlAttributeByTagNameValue("lrcDSingleLineType").toInt(); }
    /*!
     * Read Show Desktop Lrc Single Line Type.
     */
    inline int readShowDLrcLocked() const
    { return readXmlAttributeByTagNameValue("lrcDLocked").toInt(); }
    /*!
     * Read Show Desktop Lrc Locked Config.
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
    inline int readEnhancedBalance() const
    { return readXmlAttributeByTagNameValue("enhancedBalance").toInt(); }
    /*!
     * Read Enhanced Volume Balance.
     */
    inline int readEnhancedFadeEnable() const
    { return readXmlAttributeByTagNameValue("enhancedFadeEnable").toInt(); }
    /*!
     * Read Enhanced Fade Enable.
     */
    inline int readEnhancedFadeInValue() const
    { return readXmlAttributeByTagNameValue("enhancedFadeInValue").toInt(); }
    /*!
     * Read Enhanced Fade In Value.
     */
    inline int readEnhancedFadeOutValue() const
    { return readXmlAttributeByTagNameValue("enhancedFadeOutValue").toInt(); }
    /*!
     * Read Enhanced Fade Out Value.
     */
    inline int readEnhancedBS2B() const
    { return readXmlAttributeByTagNameValue("enhancedBS2B").toInt(); }
    /*!
     * Read Enhanced BS2B.
     */
    inline int readEnhancedCrossfade() const
    { return readXmlAttributeByTagNameValue("enhancedCrossfade").toInt(); }
    /*!
     * Read Enhanced Crossfade.
     */
    inline int readEnhancedStereo() const
    { return readXmlAttributeByTagNameValue("enhancedStereo").toInt(); }
    /*!
     * Read Enhanced Stereo.
     */
    inline int readEnhancedLADSPA() const
    { return readXmlAttributeByTagNameValue("enhancedLADSPA").toInt(); }
    /*!
     * Read Enhanced LADSPA.
     */
    inline int readEnhancedSOX() const
    { return readXmlAttributeByTagNameValue("enhancedSOX").toInt(); }
    /*!
     * Read Enhanced SOX.
     */

    inline int readLanguageIndex() const
    { return readXmlAttributeByTagNameValue("language").toInt(); }
    /*!
     * Read Language Index Config.
     */

    inline int readDownloadCacheLimit() const
    { return readXmlAttributeByTagNameValue("downloadCacheLimit").toInt(); }
    /*!
     * Read Download Cache Limit Config.
     */
    inline int readDownloadCacheSize() const
    { return readXmlAttributeByTagNameValue("downloadCacheSize").toInt(); }
    /*!
     * Read Download Cache Size Config.
     */

    inline QString readShowLrcFgColor() const
    { return readXmlAttributeByTagNameValue("lrcFgColor");}
    /*!
     * Read Show Lrc Fg Color Config.
     */
    inline QString readShowLrcBgColor() const
    { return readXmlAttributeByTagNameValue("lrcBgColor");}
    /*!
     * Read Show Lrc Bg Color Config.
     */
    inline QString readShowDLrcFgColor() const
    { return readXmlAttributeByTagNameValue("lrcDFgColor");}
    /*!
     * Read Show Desktop Lrc Fg Color Config.
     */
    inline QString readShowDLrcBgColor() const
    { return readXmlAttributeByTagNameValue("lrcDBgColor");}
    /*!
     * Read Show Desktop Lrc Bg Color Config.
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

#endif // MUSICXMLCONFIGMANAGER_H
