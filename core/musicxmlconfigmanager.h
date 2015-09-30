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

    inline bool readXMLConfig() { return readConfig(COFIGPATH); }
    inline bool readMusicXMLConfig(){ return readConfig(MUSICPATH); }
    void writeXMLConfig();
    void writeMusicSongsConfig(const MMusicList &musics);
    void readMusicSongsConfig(MMusicList &musics);
    inline int readMusicPlayModeConfig() const
    { return readXmlByTagNameAndAttribute("playMode").toInt(); }
    inline int readMusicPlayVolumeConfig() const
    { return readXmlByTagNameAndAttribute("playVolume").toInt(); }
    inline QString readSystemCloseConfig() const
    { return readXmlByTagNameAndAttribute("closeEvent"); }
    inline QString readSystemAutoPlayConfig() const
    { return readXmlByTagNameAndAttribute("autoPlay"); }
    inline int readEnhancedMusicConfig() const
    { return readXmlByTagNameAndAttribute("enhancedMusic").toInt(); }
    inline QString readBackgroundTheme() const
    { return readXmlByTagNameAndAttribute("bgTheme"); }
    inline QString readBackgroundTransparent() const
    { return readXmlByTagNameAndAttribute("bgTransparent"); }
    inline QString readBackgroundListTransparent() const
    { return readXmlByTagNameAndAttribute("bgListTransparent"); }
    inline QString readShowInlineLrc() const
    { return readXmlByTagNameAndAttribute("showInlineLrc"); }
    inline QString readShowDesktopLrc() const
    { return readXmlByTagNameAndAttribute("showDesktopLrc"); }
    inline int readShowLrcColor() const
    { return readXmlByTagNameAndAttribute("lrcColor").toInt(); }
    inline int readShowLrcSize() const
    { return readXmlByTagNameAndAttribute("lrcSize").toInt(); }
    inline int readShowLrcFamily() const
    { return readXmlByTagNameAndAttribute("lrcFamily").toInt(); }
    inline int readShowLrcType() const
    { return readXmlByTagNameAndAttribute("lrcType").toInt(); }
    inline int readShowLrcTransparent() const
    { return readXmlByTagNameAndAttribute("lrcTransparent").toInt(); }
    inline int readShowDLrcColor() const
    { return readXmlByTagNameAndAttribute("lrcDColor").toInt(); }
    inline int readShowDLrcSize() const
    { return readXmlByTagNameAndAttribute("lrcDSize").toInt(); }
    inline int readShowDLrcFamily() const
    { return readXmlByTagNameAndAttribute("lrcDFamily").toInt(); }
    inline int readShowDLrcType() const
    { return readXmlByTagNameAndAttribute("lrcDType").toInt(); }
    inline int readShowDLrcTransparent() const
    { return readXmlByTagNameAndAttribute("lrcDTransparent").toInt(); }
    inline int readShowDLrcLocked() const
    { return readXmlByTagNameAndAttribute("lrcDLocked").toInt(); }
    inline int readEqualizerIndex() const
    { return readXmlByTagNameAndAttribute("equalizerIndex").toInt(); }
    inline int readEqualizerEnale() const
    { return readXmlByTagNameAndAttribute("equalizerEnale").toInt(); }
    inline QString readEqualizerValue() const
    { return readXmlByTagNameAndAttribute("equalizerValue"); }
    inline int readSEqualizerIndex() const
    { return readXmlByTagNameAndAttribute("sEqualizerIndex").toInt(); }
    inline int readLanguageIndex() const
    { return readXmlByTagNameAndAttribute("language").toInt(); }

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
    void readTimeAutoConfig() const;

protected:
    MusicSongs readMusicFilePath(const QString &value) const;
    QColor readColorConfig(const QString &value) const;

};

#endif // MUSICXMLCONFIGMANAGER_H
