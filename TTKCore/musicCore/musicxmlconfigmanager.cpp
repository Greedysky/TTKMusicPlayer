#include "musicxmlconfigmanager.h"
#include "musicsettingmanager.h"

#include <QPixmap>

MusicXMLConfigManager::MusicXMLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicXMLConfigManager::readMusicSongsConfig(MusicSongsList &musics)
{
    musics << readMusicFilePath("fileNormalPath")
           << readMusicFilePath("fileLovestPath")
           << readMusicFilePath("netFilePath");

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

void MusicXMLConfigManager::writeMusicSongsConfig(const MusicSongsList &musics)
{
    if( musics.isEmpty() )
    {
        return;
    }
    //Open wirte file
    if( !writeConfig(MUSICPATH_FULL) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    //Class A
    QDomElement fileNormalPath = writeDomElement(musicPlayer, "fileNormalPath", "count", musics[0].count());
    QDomElement fileLovestPath = writeDomElement(musicPlayer, "fileLovestPath", "count", musics[1].count());
    QDomElement netFilePath = writeDomElement(musicPlayer, "netFilePath", "count", musics[2].count());

    //extend playlist init here
//    for(int i=3; i<fileNamesList.count(); ++i)
//    {
//        QDomElement extend = m_ddom->createElement("extend" + QString::number(i - 2));
//        extend.setAttribute("count",fileNamesList[i].count());
//        TTKMusicPlayer.appendChild(extend);
//    }

    //Class B
    foreach(MusicSong song, musics[0])
    {
        writeDomElementMutilText(fileNormalPath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << song.getMusicName() << song.getMusicPlayCount()
                                                   << song.getMusicTime(), song.getMusicPath());
    }

    foreach(MusicSong song, musics[1])
    {
        writeDomElementMutilText(fileLovestPath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << song.getMusicName() << song.getMusicPlayCount()
                                                   << song.getMusicTime(), song.getMusicPath());
    }

    foreach(MusicSong song, musics[2])
    {
        writeDomElementMutilText(netFilePath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << song.getMusicName() << song.getMusicPlayCount()
                                                   << song.getMusicTime(), song.getMusicPath());
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicXMLConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int autoPlayChoiced = M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toInt();
    int enhancedMusicChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    int languageIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    int closeEventChoiced = M_SETTING_PTR->value(MusicSettingManager::CloseEventChoiced).toInt();
    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    int closeNetWorkChoiced = M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt();
    int fileAssociationChoiced = M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    QString bgThemeChoiced = M_SETTING_PTR->value(MusicSettingManager::BgThemeChoiced).toString();
    QString bgTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BgTransparentChoiced).toString();
    QString bgListTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BgListTransparentChoiced).toString();

    ///////////////////////////////////////////////////////////////////////////
    int showInlineLrcChoiced = M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toInt();
    int showDesktopLrcChoiced = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toInt();
    int lrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt();
    int lrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    int lrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt();
    int lrcFamilyChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFamilyChoiced).toInt();
    QColor lrcFgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).value<QColor>();
    QColor lrcBgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcBgColorChoiced).value<QColor>();
    int lrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int DLrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt();
    int DLrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt();
    int DLrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt();
    int DLrcFamilyChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt();
    QColor DLrcFgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>();
    QColor DLrcBgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcBgColorChoiced).value<QColor>();
    int DLrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt();
    int DLrcLockedChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcLockedChoiced).toInt();
    QPoint DLrcGeometry = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometryChoiced).toPoint();

    ///////////////////////////////////////////////////////////////////////////
    int equalizerEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    QString equalizerValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString();
    int equalizerIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int timeAutoIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoIndexChoiced).toInt();
    int timeAutoPlayChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayChoiced).toInt();
    int timeAutoPlayHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt();
    int timeAutoPlaySecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt();
    int timeAutoPlayRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt();
    int timeAutoPlayItemIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt();
    int timeAutoPlaySongIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt();
    int timeAutoStopChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopChoiced).toInt();
    int timeAutoStopHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt();
    int timeAutoStopSecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt();
    int timeAutoStopRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt();
    int timeAutoShutdownChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt();
    int timeAutoShutdownHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt();
    int timeAutoShutdownSecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt();
    int timeAutoShutdownRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt();
    ///////////////////////////////////////////////////////////////////////////

    QString downloadMusicPath = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    QString downloadLrcPath = M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString();
    int downloadCacheLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt();
    int downloadCacheSize = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt();
    int downloadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt();
    int downloadServer = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    QString downloadDLoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString();
    QString downloadULoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimitChoiced).toString();
    ///////////////////////////////////////////////////////////////////////////

    //Open wirte file
    if( !writeConfig(COFIGPATH_FULL) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    //Class A
    QDomElement music = writeDom(musicPlayer, "music");
    QDomElement settings = writeDom(musicPlayer, "settings");
    QDomElement backgroundSkin = writeDom(musicPlayer, "backgroundSkin");
    QDomElement showLrc = writeDom(musicPlayer, "inlineLrc");
    QDomElement showDLrc = writeDom(musicPlayer, "desktopLrc");
    QDomElement equalizer = writeDom(musicPlayer, "equalizer");
    QDomElement timeSettings = writeDom(musicPlayer, "timeSettings");
    QDomElement downloads = writeDom(musicPlayer, "downloads");
    //Class B
    writeDomElement(music, "playMode", "value", playModeChoiced);
    writeDomElement(music, "playVolume", "value", volumeChoiced);

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(settings, "enhancedMusic", "value", enhancedMusicChoiced);
    writeDomElement(settings, "language", "value", languageIndexChoiced);
    writeDomElement(settings, "autoPlay", "value", autoPlayChoiced);
    writeDomElement(settings, "closeEvent", "value", closeEventChoiced);
    writeDomElementText(settings, "lastPlayIndex", "value", lastPlayIndexChoiced[0],
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));
    writeDomElement(settings, "closeNetwork", "value", closeNetWorkChoiced);
    writeDomElement(settings, "fileAssociation", "value", fileAssociationChoiced);

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(timeSettings, "timeAutoIndex", "value", timeAutoIndexChoiced);
    writeDomElement(timeSettings, "timeAutoPlay", "value", timeAutoPlayChoiced);
    writeDomElement(timeSettings, "timeAutoPlayHour", "value", timeAutoPlayHourChoiced);
    writeDomElement(timeSettings, "timeAutoPlaySecond", "value", timeAutoPlaySecondChoiced);
    writeDomElement(timeSettings, "timeAutoPlayRepeat", "value", timeAutoPlayRepeatChoiced);
    writeDomElement(timeSettings, "timeAutoPlayItemIndex", "value", timeAutoPlayItemIndexChoiced);
    writeDomElement(timeSettings, "timeAutoPlaySongIndex", "value", timeAutoPlaySongIndexChoiced);
    writeDomElement(timeSettings, "timeAutoStop", "value", timeAutoStopChoiced);
    writeDomElement(timeSettings, "timeAutoStopHour", "value", timeAutoStopHourChoiced);
    writeDomElement(timeSettings, "timeAutoStopSecond", "value", timeAutoStopSecondChoiced);
    writeDomElement(timeSettings, "timeAutoStopRepeat", "value", timeAutoStopRepeatChoiced);
    writeDomElement(timeSettings, "timeAutoShutdown", "value", timeAutoShutdownChoiced);
    writeDomElement(timeSettings, "timeAutoShutdownHour", "value", timeAutoShutdownHourChoiced);
    writeDomElement(timeSettings, "timeAutoShutdownSecond", "value", timeAutoShutdownSecondChoiced);
    writeDomElement(timeSettings, "timeAutoShutdownRepeat", "value", timeAutoShutdownRepeatChoiced);

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(backgroundSkin, "bgTheme", "value", bgThemeChoiced);
    writeDomElement(backgroundSkin, "bgTransparent", "value", bgTransparentChoiced);
    writeDomElement(backgroundSkin, "bgListTransparent", "value", bgListTransparentChoiced);

    ///////////////////////////////////////////////
    writeDomElement(showLrc, "showInlineLrc", "value", showInlineLrcChoiced);
    writeDomElement(showLrc, "lrcColor", "value", lrcColorChoiced);
    writeDomElement(showLrc, "lrcSize", "value", lrcSizeChoiced);
    writeDomElement(showLrc, "lrcFamily", "value", lrcFamilyChoiced);
    writeDomElement(showLrc, "lrcType", "value", lrcTypeChoiced);
    writeDomElement(showLrc, "lrcTransparent", "value", lrcTransparentChoiced);
    writeDomElement(showLrc, "lrcFgColor", "value", QString("%1,%2,%3").arg(lrcFgColorChoiced.red())
                                        .arg(lrcFgColorChoiced.green()).arg(lrcFgColorChoiced.blue()));
    writeDomElement(showLrc, "lrcBgColor", "value", QString("%1,%2,%3").arg(lrcBgColorChoiced.red())
                                        .arg(lrcBgColorChoiced.green()).arg(lrcBgColorChoiced.blue()));

    ///////////////////////////////////////////////
    writeDomElement(showDLrc, "showDesktopLrc", "value", showDesktopLrcChoiced);
    writeDomElement(showDLrc, "lrcDColor", "value", DLrcColorChoiced);
    writeDomElement(showDLrc, "lrcDSize", "value", DLrcSizeChoiced);
    writeDomElement(showDLrc, "lrcDFamily", "value", DLrcFamilyChoiced);
    writeDomElement(showDLrc, "lrcDType", "value", DLrcTypeChoiced);
    writeDomElement(showDLrc, "lrcDTransparent", "value", DLrcTransparentChoiced);
    writeDomElement(showDLrc, "lrcDFgColor", "value", QString("%1,%2,%3").arg(DLrcFgColorChoiced.red())
                                         .arg(DLrcFgColorChoiced.green()).arg(DLrcFgColorChoiced.blue()));
    writeDomElement(showDLrc, "lrcDBgColor", "value", QString("%1,%2,%3").arg(DLrcBgColorChoiced.red())
                                        .arg(DLrcBgColorChoiced.green()).arg(DLrcBgColorChoiced.blue()));

    writeDomElement(showDLrc, "lrcDLocked", "value", DLrcLockedChoiced);
    writeDomElement(showDLrc, "lrcDGeometry", "value", QString("%1,%2").arg(DLrcGeometry.x())
                                                                       .arg(DLrcGeometry.y()));

    ///////////////////////////////////////////////
    writeDomElement(equalizer, "equalizerEnale", "value", equalizerEnableChoiced);
    writeDomElement(equalizer, "equalizerIndex", "value", equalizerIndexChoiced);
    writeDomElement(equalizer, "equalizerValue", "value", equalizerValueChoiced);

    ///////////////////////////////////////////////
    writeDomElement(downloads, "downloadMusicPath", "value", downloadMusicPath);
    writeDomElement(downloads, "downloadLrcPath", "value", downloadLrcPath);
    writeDomElement(downloads, "downloadCacheLimit", "value", downloadCacheLimit);
    writeDomElement(downloads, "downloadCacheSize", "value", downloadCacheSize);
    writeDomElement(downloads, "downloadLimit", "value", downloadLimit);
    writeDomElement(downloads, "downloadServer", "value", downloadServer);
    writeDomElement(downloads, "downloadDLoadLimit", "value", downloadDLoadLimit);
    writeDomElement(downloads, "downloadULoadLimit", "value", downloadULoadLimit);
    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

MusicSongs MusicXMLConfigManager::readMusicFilePath(const QString &value) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName(value).at(0).childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        songs << MusicSong(nodelist.at(i).toElement().text(),
                           nodelist.at(i).toElement().attribute("playCount").toInt(),
                           nodelist.at(i).toElement().attribute("time"),
                           nodelist.at(i).toElement().attribute("name"));
    }
    return songs;
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
    QStringList rgb = readXmlAttributeByTagNameValue(value).split(',');
    if(rgb.count() != 3)
    {
        return QColor();
    }
    return QColor(rgb[0].toInt(),rgb[1].toInt(),rgb[2].toInt());
}

QPoint MusicXMLConfigManager::readShowDLrcGeometry() const
{
    QStringList point = readXmlAttributeByTagNameValue("lrcDGeometry").split(',');
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}

void MusicXMLConfigManager::readOtherLoadConfig() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlay").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlaySecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayRepeat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayItemIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlaySongIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStop").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdown").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());

    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDirChoiced,
                     readXmlAttributeByTagNameValue("downloadMusicPath"));
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced,
                     readXmlAttributeByTagNameValue("downloadLrcPath"));
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSizeChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced,
                     readXmlAttributeByTagNameValue("downloadServer").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit"));
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadULoadLimit"));

    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced,
                     readXmlAttributeByTagNameValue("fileAssociation"));

}
