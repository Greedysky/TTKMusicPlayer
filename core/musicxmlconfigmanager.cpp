#include "musicxmlconfigmanager.h"
#include "musicsettingmanager.h"

#include <QPixmap>

MusicXMLConfigManager::MusicXMLConfigManager(QObject *parent) :
    MusicXmlAbstract(parent)
{

}

void MusicXMLConfigManager::readMusicSongsConfig(MStringLists &fileNamesList,
                                                 MStringLists &fileUrlsList)
{
    MStringListsPair onePara;
    onePara = readMusicFilePath("fileNormalPath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;
    onePara = readMusicFilePath("fileLovestPath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;
    onePara = readMusicFilePath("netFilePath");
    fileNamesList<<onePara.first;
    fileUrlsList<<onePara.second;

    //extend playlist init here
//    for(int i=3; i<8; ++i)
//    {
//        onePara = readMusicFilePath("extend" + QString::number(i - 2));
//        if(!onePara.first.isEmpty() && !onePara.second.isEmpty())
//        {
//            fileNamesList<<onePara.first;
//            fileUrlsList<<onePara.second;
//        }
//    }
}

void MusicXMLConfigManager::writeMusicSongsConfig(const MStringLists &fileNamesList,
                                                  const MStringLists &fileUrlsList)
{
    if( fileNamesList.isEmpty() || fileUrlsList.isEmpty() )
    {
        return;
    }
    //Open wirte file
    if( !writeConfig(MUSICPATH) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement musicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(musicPlayer);
    //Class A

    QDomElement fileNormalPath = writeDomElement(musicPlayer, "fileNormalPath", "count", fileNamesList[0].count());
    QDomElement fileLovestPath = writeDomElement(musicPlayer, "fileLovestPath", "count", fileNamesList[1].count());
    QDomElement netFilePath = writeDomElement(musicPlayer, "netFilePath", "count", fileNamesList[2].count());

    //extend playlist init here
//    for(int i=3; i<fileNamesList.count(); ++i)
//    {
//        QDomElement extend = m_ddom->createElement("extend" + QString::number(i - 2));
//        extend.setAttribute("count",fileNamesList[i].count());
//        QMusicPlayer.appendChild(extend);
//    }

    //Class B
    for(int i=0; i<fileNamesList[0].count(); ++i)
    {
        writeDomEleText(fileNormalPath, "value", "name", fileNamesList[0][i], fileUrlsList[0][i]);
    }
    for(int i=0; i<fileNamesList[1].count(); ++i)
    {
        writeDomEleText(fileLovestPath, "value", "name", fileNamesList[1][i], fileUrlsList[1][i]);
    }
    for(int i=0; i<fileNamesList[2].count(); ++i)
    {
        writeDomEleText(netFilePath, "value", "name", fileNamesList[2][i], fileUrlsList[2][i]);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicXMLConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING->value(MusicSettingManager::VolumeChoiced).toInt();
    QString autoPlayChoiced = M_SETTING->value(MusicSettingManager::AutoPlayChoiced).toString();
    int play3DMusicChoiced = M_SETTING->value(MusicSettingManager::Play3DMusicChoiced).toInt();
    int languageIndexChoiced = M_SETTING->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    QString closeEventChoiced = M_SETTING->value(MusicSettingManager::CloseEventChoiced).toString();
    QStringList lastPlayIndexChoiced = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    QString bgThemeChoiced = M_SETTING->value(MusicSettingManager::BgThemeChoiced).toString();
    QString bgTransparentChoiced = M_SETTING->value(MusicSettingManager::BgTransparentChoiced).toString();
    QString showInlineLrcChoiced = M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toString();
    QString showDesktopLrcChoiced = M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toString();
    int lrcColorChoiced = M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt();
    int lrcSizeChoiced = M_SETTING->value(MusicSettingManager::LrcSizeChoiced).toInt();
    int lrcTypeChoiced = M_SETTING->value(MusicSettingManager::LrcTypeChoiced).toInt();
    int lrcFamilyChoiced = M_SETTING->value(MusicSettingManager::LrcFamilyChoiced).toInt();
    QColor lrcFgColorChoiced = M_SETTING->value(MusicSettingManager::LrcFgColorChoiced).value<QColor>();
    QColor lrcBgColorChoiced = M_SETTING->value(MusicSettingManager::LrcBgColorChoiced).value<QColor>();
    int lrcTransparentChoiced = M_SETTING->value(MusicSettingManager::LrcColorTransChoiced).toInt();
    int DLrcColorChoiced = M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt();
    int DLrcSizeChoiced = M_SETTING->value(MusicSettingManager::DLrcSizeChoiced).toInt();
    int DLrcTypeChoiced = M_SETTING->value(MusicSettingManager::DLrcTypeChoiced).toInt();
    int DLrcFamilyChoiced = M_SETTING->value(MusicSettingManager::DLrcFamilyChoiced).toInt();
    QColor DLrcFgColorChoiced = M_SETTING->value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>();
    QColor DLrcBgColorChoiced = M_SETTING->value(MusicSettingManager::DLrcBgColorChoiced).value<QColor>();
    int DLrcTransparentChoiced = M_SETTING->value(MusicSettingManager::DLrcColorTransChoiced).toInt();
    int DLrcLockedChoiced = M_SETTING->value(MusicSettingManager::DLrcLockedChoiced).toInt();
    QRect DLrcGeometry = M_SETTING->value(MusicSettingManager::DLrcGeometryChoiced).toRect();
    int equalizerEnableChoiced = M_SETTING->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    QString equalizerValueChoiced = M_SETTING->value(MusicSettingManager::EqualizerValueChoiced).toString();
    int equalizerIndexChoiced = M_SETTING->value(MusicSettingManager::EqualizerIndexChoiced).toInt();
    int sEqualizerIndexChoiced = M_SETTING->value(MusicSettingManager::SequalizerIndexChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int timeAutoIndexChoiced = M_SETTING->value(MusicSettingManager::TimerAutoIndexChoiced).toInt();
    int timeAutoPlayChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlayChoiced).toInt();
    int timeAutoPlayHourChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt();
    int timeAutoPlaySecondChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt();
    int timeAutoPlayRepeatChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt();
    int timeAutoPlayItemIndexChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt();
    int timeAutoPlaySongIndexChoiced = M_SETTING->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt();
    int timeAutoStopChoiced = M_SETTING->value(MusicSettingManager::TimerAutoStopChoiced).toInt();
    int timeAutoStopHourChoiced = M_SETTING->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt();
    int timeAutoStopSecondChoiced = M_SETTING->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt();
    int timeAutoStopRepeatChoiced = M_SETTING->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt();
    int timeAutoShutdownChoiced = M_SETTING->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt();
    int timeAutoShutdownHourChoiced = M_SETTING->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt();
    int timeAutoShutdownSecondChoiced = M_SETTING->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt();
    int timeAutoShutdownRepeatChoiced = M_SETTING->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt();
    ///////////////////////////////////////////////////////////////////////////

    //Open wirte file
    if( !writeConfig(COFIGPATH) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    //Class A
    QDomElement music = m_ddom->createElement("music");
    QMusicPlayer.appendChild(music);
    QDomElement settings = m_ddom->createElement("settings");
    QMusicPlayer.appendChild(settings);
    QDomElement backgroundSkin = m_ddom->createElement("backgroundSkin");
    QMusicPlayer.appendChild(backgroundSkin);
    QDomElement showLrc = m_ddom->createElement("inlineLrc");
    QMusicPlayer.appendChild(showLrc);
    QDomElement showDLrc = m_ddom->createElement("desktopLrc");
    QMusicPlayer.appendChild(showDLrc);
    QDomElement equalizer = m_ddom->createElement("equalizer");
    QMusicPlayer.appendChild(equalizer);

    //Class B
    QDomElement playMode = m_ddom->createElement("playMode");
    playMode.setAttribute("value",playModeChoiced);
    music.appendChild(playMode);
    QDomElement playVolume = m_ddom->createElement("playVolume");
    playVolume.setAttribute("value",volumeChoiced);
    music.appendChild(playVolume);

    QDomElement play3DMusic = m_ddom->createElement("play3DMusic");
    play3DMusic.setAttribute("value",play3DMusicChoiced);
    settings.appendChild(play3DMusic);
    QDomElement language = m_ddom->createElement("language");
    language.setAttribute("value",languageIndexChoiced);
    settings.appendChild(language);
    QDomElement autoPlay = m_ddom->createElement("autoPlay");
    autoPlay.setAttribute("value",autoPlayChoiced);
    settings.appendChild(autoPlay);
    QDomElement closeEvent = m_ddom->createElement("closeEvent");
    closeEvent.setAttribute("value",closeEventChoiced);
    settings.appendChild(closeEvent);
    QDomElement lastPlayIndex = m_ddom->createElement("lastPlayIndex");
    lastPlayIndex.setAttribute("value",lastPlayIndexChoiced[0]);
    QDomText lastPlayIndextext = m_ddom->createTextNode(QString("%1,%2").arg(
                lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));
    lastPlayIndex.appendChild(lastPlayIndextext);
    settings.appendChild(lastPlayIndex);
    ///////////////////////////////////////////////////////////////////////////
    QDomElement timeAutoIndex = m_ddom->createElement("timeAutoIndex");
    timeAutoIndex.setAttribute("value",timeAutoIndexChoiced);
    settings.appendChild(timeAutoIndex);
    QDomElement timeAutoPlay = m_ddom->createElement("timeAutoPlay");
    timeAutoPlay.setAttribute("value",timeAutoPlayChoiced);
    settings.appendChild(timeAutoPlay);
    QDomElement timeAutoPlayHour = m_ddom->createElement("timeAutoPlayHour");
    timeAutoPlayHour.setAttribute("value",timeAutoPlayHourChoiced);
    settings.appendChild(timeAutoPlayHour);
    QDomElement timeAutoPlaySecond = m_ddom->createElement("timeAutoPlaySecond");
    timeAutoPlaySecond.setAttribute("value",timeAutoPlaySecondChoiced);
    settings.appendChild(timeAutoPlaySecond);
    QDomElement timeAutoPlayRepeat = m_ddom->createElement("timeAutoPlayRepeat");
    timeAutoPlayRepeat.setAttribute("value",timeAutoPlayRepeatChoiced);
    settings.appendChild(timeAutoPlayRepeat);
    QDomElement timeAutoPlayItemIndex = m_ddom->createElement("timeAutoPlayItemIndex");
    timeAutoPlayItemIndex.setAttribute("value",timeAutoPlayItemIndexChoiced);
    settings.appendChild(timeAutoPlayItemIndex);
    QDomElement timeAutoPlaySongIndex = m_ddom->createElement("timeAutoPlaySongIndex");
    timeAutoPlaySongIndex.setAttribute("value",timeAutoPlaySongIndexChoiced);
    settings.appendChild(timeAutoPlaySongIndex);
    QDomElement timeAutoStop = m_ddom->createElement("timeAutoStop");
    timeAutoStop.setAttribute("value",timeAutoStopChoiced);
    settings.appendChild(timeAutoStop);
    QDomElement timeAutoStopHour = m_ddom->createElement("timeAutoStopHour");
    timeAutoStopHour.setAttribute("value",timeAutoStopHourChoiced);
    settings.appendChild(timeAutoStopHour);
    QDomElement timeAutoStopSecond = m_ddom->createElement("timeAutoStopSecond");
    timeAutoStopSecond.setAttribute("value",timeAutoStopSecondChoiced);
    settings.appendChild(timeAutoStopSecond);
    QDomElement timeAutoStopRepeat = m_ddom->createElement("timeAutoStopRepeat");
    timeAutoStopRepeat.setAttribute("value",timeAutoStopRepeatChoiced);
    settings.appendChild(timeAutoStopRepeat);
    QDomElement timeAutoShutdown = m_ddom->createElement("timeAutoShutdown");
    timeAutoShutdown.setAttribute("value",timeAutoShutdownChoiced);
    settings.appendChild(timeAutoShutdown);
    QDomElement timeAutoShutdownHour = m_ddom->createElement("timeAutoShutdownHour");
    timeAutoShutdownHour.setAttribute("value",timeAutoShutdownHourChoiced);
    settings.appendChild(timeAutoShutdownHour);
    QDomElement timeAutoShutdownSecond = m_ddom->createElement("timeAutoShutdownSecond");
    timeAutoShutdownSecond.setAttribute("value",timeAutoShutdownSecondChoiced);
    settings.appendChild(timeAutoShutdownSecond);
    QDomElement timeAutoShutdownRepeat = m_ddom->createElement("timeAutoShutdownRepeat");
    timeAutoShutdownRepeat.setAttribute("value",timeAutoShutdownRepeatChoiced);
    settings.appendChild(timeAutoShutdownRepeat);
    ///////////////////////////////////////////////////////////////////////////

    QDomElement bgTheme = m_ddom->createElement("bgTheme");
    bgTheme.setAttribute("value",bgThemeChoiced);
    backgroundSkin.appendChild(bgTheme);
    QDomElement bgTransparent = m_ddom->createElement("bgTransparent");
    bgTransparent.setAttribute("value",bgTransparentChoiced);
    backgroundSkin.appendChild(bgTransparent);

    ///////////////////////////////////////////////
    QDomElement showInlineLrc = m_ddom->createElement("showInlineLrc");
    showInlineLrc.setAttribute("value",showInlineLrcChoiced);
    showLrc.appendChild(showInlineLrc);
    QDomElement lrcColor = m_ddom->createElement("lrcColor");
    lrcColor.setAttribute("value",lrcColorChoiced);
    showLrc.appendChild(lrcColor);
    QDomElement lrcSize = m_ddom->createElement("lrcSize");
    lrcSize.setAttribute("value",lrcSizeChoiced);
    showLrc.appendChild(lrcSize);
    QDomElement lrcFamily = m_ddom->createElement("lrcFamily");
    lrcFamily.setAttribute("value",lrcFamilyChoiced);
    showLrc.appendChild(lrcFamily);
    QDomElement lrcType = m_ddom->createElement("lrcType");
    lrcType.setAttribute("value",lrcTypeChoiced);
    showLrc.appendChild(lrcType);
    QDomElement lrcTransparent = m_ddom->createElement("lrcTransparent");
    lrcTransparent.setAttribute("value",lrcTransparentChoiced);
    showLrc.appendChild(lrcTransparent);
    QDomElement lrcFgColor = m_ddom->createElement("lrcFgColor");
    lrcFgColor.setAttribute("value",QString("%1,%2,%3").arg(lrcFgColorChoiced.red())
                          .arg(lrcFgColorChoiced.green()).arg(lrcFgColorChoiced.blue()));
    showLrc.appendChild(lrcFgColor);
    QDomElement lrcBgColor = m_ddom->createElement("lrcBgColor");
    lrcBgColor.setAttribute("value",QString("%1,%2,%3").arg(lrcBgColorChoiced.red())
                          .arg(lrcBgColorChoiced.green()).arg(lrcBgColorChoiced.blue()));
    showLrc.appendChild(lrcBgColor);

    ///////////////////////////////////////////////
    QDomElement showDesktopLrc = m_ddom->createElement("showDesktopLrc");
    showDesktopLrc.setAttribute("value",showDesktopLrcChoiced);
    showDLrc.appendChild(showDesktopLrc);
    QDomElement lrcDColor = m_ddom->createElement("lrcDColor");
    lrcDColor.setAttribute("value",DLrcColorChoiced);
    showDLrc.appendChild(lrcDColor);
    QDomElement lrcDSize = m_ddom->createElement("lrcDSize");
    lrcDSize.setAttribute("value",DLrcSizeChoiced);
    showDLrc.appendChild(lrcDSize);
    QDomElement lrcDFamily = m_ddom->createElement("lrcDFamily");
    lrcDFamily.setAttribute("value",DLrcFamilyChoiced);
    showDLrc.appendChild(lrcDFamily);
    QDomElement lrcDType = m_ddom->createElement("lrcDType");
    lrcDType.setAttribute("value",DLrcTypeChoiced);
    showDLrc.appendChild(lrcDType);
    QDomElement lrcDTransparent = m_ddom->createElement("lrcDTransparent");
    lrcDTransparent.setAttribute("value",DLrcTransparentChoiced);
    showDLrc.appendChild(lrcDTransparent);
    QDomElement lrcDFgColor = m_ddom->createElement("lrcDFgColor");
    lrcDFgColor.setAttribute("value",QString("%1,%2,%3").arg(DLrcFgColorChoiced.red())
                          .arg(DLrcFgColorChoiced.green()).arg(DLrcFgColorChoiced.blue()));
    showDLrc.appendChild(lrcDFgColor);
    QDomElement lrcDBgColor = m_ddom->createElement("lrcDBgColor");
    lrcDBgColor.setAttribute("value",QString("%1,%2,%3").arg(DLrcBgColorChoiced.red())
                          .arg(DLrcBgColorChoiced.green()).arg(DLrcBgColorChoiced.blue()));
    showDLrc.appendChild(lrcDBgColor);
    QDomElement lrcDLocked = m_ddom->createElement("lrcDLocked");
    lrcDLocked.setAttribute("value",DLrcLockedChoiced);
    showDLrc.appendChild(lrcDLocked);
    QDomElement lrcDGeometry = m_ddom->createElement("lrcDGeometry");
    lrcDGeometry.setAttribute("value",QString("%1,%2,%3,%4").arg(DLrcGeometry.left())
                           .arg(DLrcGeometry.top()).arg(DLrcGeometry.width())
                           .arg(DLrcGeometry.height()));
    showDLrc.appendChild(lrcDGeometry);

    QDomElement equalizerEnale = m_ddom->createElement("equalizerEnale");
    equalizerEnale.setAttribute("value",equalizerEnableChoiced);
    equalizer.appendChild(equalizerEnale);
    QDomElement equalizerIndex = m_ddom->createElement("equalizerIndex");
    equalizerIndex.setAttribute("value",equalizerIndexChoiced);
    equalizer.appendChild(equalizerIndex);
    QDomElement sEqualizerIndex = m_ddom->createElement("sEqualizerIndex");
    sEqualizerIndex.setAttribute("value",sEqualizerIndexChoiced);
    equalizer.appendChild(sEqualizerIndex);
    QDomElement equalizerValue = m_ddom->createElement("equalizerValue");
    equalizerValue.setAttribute("value",equalizerValueChoiced);
    equalizer.appendChild(equalizerValue);
    ///////////////////////////////////////////////

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

MStringListsPair MusicXMLConfigManager::readMusicFilePath(const QString &value) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(value).at(0).childNodes();
    QStringList name;
    QStringList url;
    for(int i=0; i<nodelist.count(); i++)
    {
       name.append(nodelist.at(i).toElement().attribute("name"));
       url.append(nodelist.at(i).toElement().text());
    }
    return MStringListsPair(name,url);
}

void MusicXMLConfigManager::readSystemLastPlayIndexConfig(QStringList &key) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("lastPlayIndex");
    QDomElement element = nodelist.at(0).toElement();
    key<<element.attribute("value")<<element.text().split(',');
    Q_ASSERT(key.count() == 3);
    if(key.count() != 3)
    {
        key.insert(0, "0");
    }
}

QColor MusicXMLConfigManager::readColorConfig(const QString &value) const
{
    QStringList rgb = readXmlByTagNameAndAttribute(value).split(',');
    if(rgb.count() != 3)
    {
        return QColor();
    }
    return QColor(rgb[0].toInt(),rgb[1].toInt(),rgb[2].toInt());
}

QRect MusicXMLConfigManager::readShowDLrcGeometry() const
{
    QStringList geometry = readXmlByTagNameAndAttribute("lrcDGeometry").split(',');
    if(geometry.count() != 4)
    {
        return QRect();
    }
    return QRect(geometry[0].toInt(),geometry[1].toInt(),
                 geometry[2].toInt(),geometry[3].toInt() );
}

void MusicXMLConfigManager::readTimeAutoConfig() const
{
    M_SETTING->setValue(MusicSettingManager::TimerAutoIndexChoiced,
                     readXmlByTagNameAndAttribute("timeAutoIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlay").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayHourChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlayHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySecondChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlaySecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlayRepeat").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlayItemIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced,
                     readXmlByTagNameAndAttribute("timeAutoPlaySongIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopChoiced,
                     readXmlByTagNameAndAttribute("timeAutoStop").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopHourChoiced,
                     readXmlByTagNameAndAttribute("timeAutoStopHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopSecondChoiced,
                     readXmlByTagNameAndAttribute("timeAutoStopSecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopRepeatChoiced,
                     readXmlByTagNameAndAttribute("timeAutoStopRepeat").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownChoiced,
                     readXmlByTagNameAndAttribute("timeAutoShutdown").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownHourChoiced,
                     readXmlByTagNameAndAttribute("timeAutoShutdownHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced,
                     readXmlByTagNameAndAttribute("timeAutoShutdownSecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced,
                     readXmlByTagNameAndAttribute("timeAutoShutdownRepeat").toInt());
}
