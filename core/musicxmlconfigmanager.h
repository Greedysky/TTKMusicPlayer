#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include <QPair>
#include <QColor>
#include "musicxmlabstract.h"

class MUSIC_EXPORT MusicXMLConfigManager : public MusicXmlAbstract
{
public:
    explicit MusicXMLConfigManager(QObject *parent = 0);

    inline bool readXMLConfig() { return readConfig(COFIGPATH); }
    inline bool readMusicXMLConfig(){ return readConfig(MUSICPATH); }
    void writeXMLConfig();
    void writeMusicSongsConfig(const MStringLists &, const MStringLists &);
    void readMusicSongsConfig(MStringLists &, MStringLists &);
    inline int readMusicPlayModeConfig()
    { return readXmlByTagNameAndAttribute("playMode").toInt(); }
    inline int readMusicPlayVolumeConfig()
    { return readXmlByTagNameAndAttribute("playVolume").toInt(); }
    inline QString readSystemCloseConfig()
    { return readXmlByTagNameAndAttribute("closeEvent"); }
    inline QString readSystemAutoPlayConfig()
    { return readXmlByTagNameAndAttribute("autoPlay"); }
    inline int read3DMusicPlayConfig()
    { return readXmlByTagNameAndAttribute("play3DMusic").toInt(); }
    inline QString readBackgroundTheme()
    { return readXmlByTagNameAndAttribute("bgTheme"); }
    inline QString readBackgroundTransparent()
    { return readXmlByTagNameAndAttribute("bgTransparent"); }
    inline QString readShowInlineLrc()
    { return readXmlByTagNameAndAttribute("showInlineLrc"); }
    inline QString readShowDesktopLrc()
    { return readXmlByTagNameAndAttribute("showDesktopLrc"); }
    inline int readShowLrcColor()
    { return readXmlByTagNameAndAttribute("lrcColor").toInt(); }
    inline int readShowLrcSize()
    { return readXmlByTagNameAndAttribute("lrcSize").toInt(); }
    inline int readShowLrcFamily()
    { return readXmlByTagNameAndAttribute("lrcFamily").toInt(); }
    inline int readShowLrcType()
    { return readXmlByTagNameAndAttribute("lrcType").toInt(); }
    inline int readShowLrcTransparent()
    { return readXmlByTagNameAndAttribute("lrcTransparent").toInt(); }
    inline int readShowDLrcColor()
    { return readXmlByTagNameAndAttribute("lrcDColor").toInt(); }
    inline int readShowDLrcSize()
    { return readXmlByTagNameAndAttribute("lrcDSize").toInt(); }
    inline int readShowDLrcFamily()
    { return readXmlByTagNameAndAttribute("lrcDFamily").toInt(); }
    inline int readShowDLrcType()
    { return readXmlByTagNameAndAttribute("lrcDType").toInt(); }
    inline int readShowDLrcTransparent()
    { return readXmlByTagNameAndAttribute("lrcDTransparent").toInt(); }
    inline int readShowDLrcLocked()
    { return readXmlByTagNameAndAttribute("lrcDLocked").toInt(); }
    inline int readEqualizerIndex()
    { return readXmlByTagNameAndAttribute("equalizerIndex").toInt(); }
    inline int readEqualizerEnale()
    { return readXmlByTagNameAndAttribute("equalizerEnale").toInt(); }
    inline QString readEqualizerValue()
    { return readXmlByTagNameAndAttribute("equalizerValue"); }
    inline int readSEqualizerIndex()
    { return readXmlByTagNameAndAttribute("sEqualizerIndex").toInt(); }
    inline int readLanguageIndex()
    { return readXmlByTagNameAndAttribute("language").toInt(); }

    inline QColor readShowLrcFgColor() { return readColorConfig("lrcFgColor");}
    inline QColor readShowLrcBgColor() { return readColorConfig("lrcBgColor");}
    inline QColor readShowDLrcFgColor() { return readColorConfig("lrcDFgColor");}
    inline QColor readShowDLrcBgColor() { return readColorConfig("lrcDBgColor");}

    void readSystemLastPlayIndexConfig(QStringList& key);
    QRect readShowDLrcGeometry();
    void readTimeAutoConfig();

protected:
    MStringListsPair readMusicFilePath(const QString&);
    QColor readColorConfig(const QString& value);

};

#endif // MUSICXMLCONFIGMANAGER_H
