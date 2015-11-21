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
    if( !writeConfig(MUSICPATH_AL) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");
    //Class A
    QDomElement fileNormalPath = writeDomElement(musicPlayer, "fileNormalPath", "count", musics[0].count());
    QDomElement fileLovestPath = writeDomElement(musicPlayer, "fileLovestPath", "count", musics[1].count());
    QDomElement netFilePath = writeDomElement(musicPlayer, "netFilePath", "count", musics[2].count());

    //extend playlist init here
//    for(int i=3; i<fileNamesList.count(); ++i)
//    {
//        QDomElement extend = m_ddom->createElement("extend" + QString::number(i - 2));
//        extend.setAttribute("count",fileNamesList[i].count());
//        QMusicPlayer.appendChild(extend);
//    }

    //Class B

    for(int i=0; i<musics[0].count(); ++i)
    {
        writeDomElementMutilText(fileNormalPath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << musics[0][i].getMusicName() << musics[0][i].getMusicPlayCount()
                                                   << musics[0][i].getMusicTime(), musics[0][i].getMusicPath());
    }
    for(int i=0; i<musics[1].count(); ++i)
    {
        writeDomElementMutilText(fileLovestPath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << musics[1][i].getMusicName() << musics[1][i].getMusicPlayCount()
                                                   << musics[1][i].getMusicTime(), musics[1][i].getMusicPath());
    }
    for(int i=0; i<musics[2].count(); ++i)
    {
        writeDomElementMutilText(netFilePath, "value", QStringList() << "name" << "playCount" << "time",
                                 QList<QVariant>() << musics[2][i].getMusicName() << musics[2][i].getMusicPlayCount()
                                                   << musics[2][i].getMusicTime(), musics[2][i].getMusicPath());
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicXMLConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING->value(MusicSettingManager::VolumeChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    QString autoPlayChoiced = M_SETTING->value(MusicSettingManager::AutoPlayChoiced).toString();
    int enhancedMusicChoiced = M_SETTING->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    int languageIndexChoiced = M_SETTING->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    QString closeEventChoiced = M_SETTING->value(MusicSettingManager::CloseEventChoiced).toString();
    QStringList lastPlayIndexChoiced = M_SETTING->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();
    int closeNetWorkChoiced = M_SETTING->value(MusicSettingManager::CloseNetWorkChoiced).toInt();
    int fileAssociationChoiced = M_SETTING->value(MusicSettingManager::FileAssociationChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    QString bgThemeChoiced = M_SETTING->value(MusicSettingManager::BgThemeChoiced).toString();
    QString bgTransparentChoiced = M_SETTING->value(MusicSettingManager::BgTransparentChoiced).toString();
    QString bgListTransparentChoiced = M_SETTING->value(MusicSettingManager::BgListTransparentChoiced).toString();

    ///////////////////////////////////////////////////////////////////////////
    QString showInlineLrcChoiced = M_SETTING->value(MusicSettingManager::ShowInlineLrcChoiced).toString();
    QString showDesktopLrcChoiced = M_SETTING->value(MusicSettingManager::ShowDesktopLrcChoiced).toString();
    int lrcColorChoiced = M_SETTING->value(MusicSettingManager::LrcColorChoiced).toInt();
    int lrcSizeChoiced = M_SETTING->value(MusicSettingManager::LrcSizeChoiced).toInt();
    int lrcTypeChoiced = M_SETTING->value(MusicSettingManager::LrcTypeChoiced).toInt();
    int lrcFamilyChoiced = M_SETTING->value(MusicSettingManager::LrcFamilyChoiced).toInt();
    QColor lrcFgColorChoiced = M_SETTING->value(MusicSettingManager::LrcFgColorChoiced).value<QColor>();
    QColor lrcBgColorChoiced = M_SETTING->value(MusicSettingManager::LrcBgColorChoiced).value<QColor>();
    int lrcTransparentChoiced = M_SETTING->value(MusicSettingManager::LrcColorTransChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int DLrcColorChoiced = M_SETTING->value(MusicSettingManager::DLrcColorChoiced).toInt();
    int DLrcSizeChoiced = M_SETTING->value(MusicSettingManager::DLrcSizeChoiced).toInt();
    int DLrcTypeChoiced = M_SETTING->value(MusicSettingManager::DLrcTypeChoiced).toInt();
    int DLrcFamilyChoiced = M_SETTING->value(MusicSettingManager::DLrcFamilyChoiced).toInt();
    QColor DLrcFgColorChoiced = M_SETTING->value(MusicSettingManager::DLrcFgColorChoiced).value<QColor>();
    QColor DLrcBgColorChoiced = M_SETTING->value(MusicSettingManager::DLrcBgColorChoiced).value<QColor>();
    int DLrcTransparentChoiced = M_SETTING->value(MusicSettingManager::DLrcColorTransChoiced).toInt();
    int DLrcLockedChoiced = M_SETTING->value(MusicSettingManager::DLrcLockedChoiced).toInt();
    QRect DLrcGeometry = M_SETTING->value(MusicSettingManager::DLrcGeometryChoiced).toRect();

    ///////////////////////////////////////////////////////////////////////////
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

    QString downloadMusicPath = M_SETTING->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    QString downloadLrcPath = M_SETTING->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString();
    int downloadCacheLimit = M_SETTING->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt();
    int downloadCacheSize = M_SETTING->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt();
    int downloadLimit = M_SETTING->value(MusicSettingManager::DownloadLimitChoiced).toInt();
    int downloadDLoadLimit = M_SETTING->value(MusicSettingManager::DownloadDLoadLimitChoiced).toInt();
    int downloadULoadLimit = M_SETTING->value(MusicSettingManager::DownloadULoadLimitChoiced).toInt();
    ///////////////////////////////////////////////////////////////////////////

    //Open wirte file
    if( !writeConfig(COFIGPATH_AL) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");
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
    writeDomElement(showDLrc, "lrcDGeometry", "value", QString("%1,%2,%3,%4").arg(DLrcGeometry.left())
                                                     .arg(DLrcGeometry.top()).arg(DLrcGeometry.width())
                                                     .arg(DLrcGeometry.height()));

    ///////////////////////////////////////////////
    writeDomElement(equalizer, "equalizerEnale", "value", equalizerEnableChoiced);
    writeDomElement(equalizer, "equalizerIndex", "value", equalizerIndexChoiced);
    writeDomElement(equalizer, "sEqualizerIndex", "value", sEqualizerIndexChoiced);
    writeDomElement(equalizer, "equalizerValue", "value", equalizerValueChoiced);

    ///////////////////////////////////////////////
    writeDomElement(downloads, "downloadMusicPath", "value", downloadMusicPath);
    writeDomElement(downloads, "downloadLrcPath", "value", downloadLrcPath);
    writeDomElement(downloads, "downloadCacheLimit", "value", downloadCacheLimit);
    writeDomElement(downloads, "downloadCacheSize", "value", downloadCacheSize);
    writeDomElement(downloads, "downloadLimit", "value", downloadLimit);
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

QRect MusicXMLConfigManager::readShowDLrcGeometry() const
{
    QStringList geometry = readXmlAttributeByTagNameValue("lrcDGeometry").split(',');
    if(geometry.count() != 4)
    {
        return QRect();
    }
    return QRect(geometry[0].toInt(),geometry[1].toInt(),
                 geometry[2].toInt(),geometry[3].toInt() );
}

void MusicXMLConfigManager::readOtherLoadConfig() const
{
    M_SETTING->setValue(MusicSettingManager::TimerAutoIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlay").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlaySecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayRepeat").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlayItemIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlayItemIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoPlaySongIndexChoiced,
                     readXmlAttributeByTagNameValue("timeAutoPlaySongIndex").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStop").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopSecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoStopRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdown").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownHourChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownSecondChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    M_SETTING->setValue(MusicSettingManager::TimerAutoShutdownRepeatChoiced,
                     readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());

    M_SETTING->setValue(MusicSettingManager::DownloadMusicPathDirChoiced,
                     readXmlAttributeByTagNameValue("downloadMusicPath"));
    M_SETTING->setValue(MusicSettingManager::DownloadLrcPathDirChoiced,
                     readXmlAttributeByTagNameValue("downloadLrcPath"));
    M_SETTING->setValue(MusicSettingManager::DownloadCacheLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheLimit").toInt());
    M_SETTING->setValue(MusicSettingManager::DownloadCacheSizeChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    M_SETTING->setValue(MusicSettingManager::DownloadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    M_SETTING->setValue(MusicSettingManager::DownloadDLoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit").toInt());
    M_SETTING->setValue(MusicSettingManager::DownloadULoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadULoadLimit").toInt());

    M_SETTING->setValue(MusicSettingManager::FileAssociationChoiced,
                     readXmlAttributeByTagNameValue("fileAssociation"));

}
