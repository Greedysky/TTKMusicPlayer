#ifndef MUSICXMLCONFIGMANAGER_H
#define MUSICXMLCONFIGMANAGER_H

#include <QPair>
#include <QColor>
#include "musicxmlabstract.h"

class MUSIC_CORE_EXPORT MusicXMLConfigManager : public MusicXmlAbstract
{
public:
    explicit MusicXMLConfigManager(QObject *parent = 0);

    inline bool readXMLConfig() { return readConfig(COFIGPATH); }
    inline bool readMusicXMLConfig(){ return readConfig(MUSICPATH); }
    void writeXMLConfig();
    void writeMusicSongsConfig(const MStringLists &fileNamesList,
                               const MStringLists &fileUrlsList);
    void readMusicSongsConfig(MStringLists &fileNamesList,
                              MStringLists &fileUrlsList);
    inline int readMusicPlayModeConfig() const
    { return readXmlByTagNameAndAttribute("playMode").toInt(); }
    inline int readMusicPlayVolumeConfig() const
    { return readXmlByTagNameAndAttribute("playVolume").toInt(); }
    inline QString readSystemCloseConfig() const
    { return readXmlByTagNameAndAttribute("closeEvent"); }
    inline QString readSystemAutoPlayConfig() const
    { return readXmlByTagNameAndAttribute("autoPlay"); }
    inline int read3DMusicPlayConfig() const
    { return readXmlByTagNameAndAttribute("play3DMusic").toInt(); }
    inline QString readBackgroundTheme() const
    { return readXmlByTagNameAndAttribute("bgTheme"); }
    inline QString readBackgroundTransparent() const
    { return readXmlByTagNameAndAttribute("bgTransparent"); }
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
    MStringListsPair readMusicFilePath(const QString &value) const;
    QColor readColorConfig(const QString &value) const;

};

#endif // MUSICXMLCONFIGMANAGER_H
