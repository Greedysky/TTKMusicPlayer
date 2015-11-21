#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QPair>
#include <QColor>
#include "musicabstractxml.h"

class MUSIC_CORE_EXPORT MusicXMLConfigManager : public MusicAbstractXml
{
public:
    explicit MusicXMLConfigManager(QObject *parent = 0);

    inline bool readXMLConfig() { return readConfig(COFIGPATH_AL); }
    inline bool readMusicXMLConfig(){ return readConfig(MUSICPATH_AL); }
    void writeXMLConfig();
    void writeMusicSongsConfig(const MusicSongsList &musics);
    void readMusicSongsConfig(MusicSongsList &musics);
    inline int readMusicPlayModeConfig() const
    { return readXmlAttributeByTagNameValue("playMode").toInt(); }
    inline int readMusicPlayVolumeConfig() const
    { return readXmlAttributeByTagNameValue("playVolume").toInt(); }
    inline QString readSystemCloseConfig() const
    { return readXmlAttributeByTagNameValue("closeEvent"); }
    inline int readCloseNetworkConfig() const
    { return readXmlAttributeByTagNameValue("closeNetwork").toInt(); }
    inline QString readSystemAutoPlayConfig() const
    { return readXmlAttributeByTagNameValue("autoPlay"); }
    inline int readEnhancedMusicConfig() const
    { return readXmlAttributeByTagNameValue("enhancedMusic").toInt(); }
    inline QString readBackgroundTheme() const
    { return readXmlAttributeByTagNameValue("bgTheme"); }
    inline QString readBackgroundTransparent() const
    { return readXmlAttributeByTagNameValue("bgTransparent"); }
    inline QString readBackgroundListTransparent() const
    { return readXmlAttributeByTagNameValue("bgListTransparent"); }
    inline QString readShowInlineLrc() const
    { return readXmlAttributeByTagNameValue("showInlineLrc"); }
    inline QString readShowDesktopLrc() const
    { return readXmlAttributeByTagNameValue("showDesktopLrc"); }
    inline int readShowLrcColor() const
    { return readXmlAttributeByTagNameValue("lrcColor").toInt(); }
    inline int readShowLrcSize() const
    { return readXmlAttributeByTagNameValue("lrcSize").toInt(); }
    inline int readShowLrcFamily() const
    { return readXmlAttributeByTagNameValue("lrcFamily").toInt(); }
    inline int readShowLrcType() const
    { return readXmlAttributeByTagNameValue("lrcType").toInt(); }
    inline int readShowLrcTransparent() const
    { return readXmlAttributeByTagNameValue("lrcTransparent").toInt(); }
    inline int readShowDLrcColor() const
    { return readXmlAttributeByTagNameValue("lrcDColor").toInt(); }
    inline int readShowDLrcSize() const
    { return readXmlAttributeByTagNameValue("lrcDSize").toInt(); }
    inline int readShowDLrcFamily() const
    { return readXmlAttributeByTagNameValue("lrcDFamily").toInt(); }
    inline int readShowDLrcType() const
    { return readXmlAttributeByTagNameValue("lrcDType").toInt(); }
    inline int readShowDLrcTransparent() const
    { return readXmlAttributeByTagNameValue("lrcDTransparent").toInt(); }
    inline int readShowDLrcLocked() const
    { return readXmlAttributeByTagNameValue("lrcDLocked").toInt(); }
    inline int readEqualizerIndex() const
    { return readXmlAttributeByTagNameValue("equalizerIndex").toInt(); }
    inline int readEqualizerEnale() const
    { return readXmlAttributeByTagNameValue("equalizerEnale").toInt(); }
    inline QString readEqualizerValue() const
    { return readXmlAttributeByTagNameValue("equalizerValue"); }
    inline int readSEqualizerIndex() const
    { return readXmlAttributeByTagNameValue("sEqualizerIndex").toInt(); }
    inline int readLanguageIndex() const
    { return readXmlAttributeByTagNameValue("language").toInt(); }
    inline int readDownloadCacheLimit() const
    { return readXmlAttributeByTagNameValue("downloadCacheLimit").toInt(); }
    inline int readDownloadCacheSize() const
    { return readXmlAttributeByTagNameValue("downloadCacheSize").toInt(); }

    inline QColor readShowLrcFgColor() const
    { return readColorConfig("lrcFgColor");}
    inline QColor readShowLrcBgColor() const
    { return readColorConfig("lrcBgColor");}
    inline QColor readShowDLrcFgColor() const
    { return readColorConfig("lrcDFgColor");}
    inline QColor readShowDLrcBgColor() const
    { return readColorConfig("lrcDBgColor");}

    void readSystemLastPlayIndexConfig(QStringList &key) const;
    QRect readShowDLrcGeometry() const;
    void readOtherLoadConfig() const;

protected:
    MusicSongs readMusicFilePath(const QString &value) const;
    QColor readColorConfig(const QString &value) const;

};

#endif // MUSICXMLCONFIGMANAGER_H
