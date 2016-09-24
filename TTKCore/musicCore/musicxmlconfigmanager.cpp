#include "musicxmlconfigmanager.h"
#include "musicsettingmanager.h"

#include <QRect>

MusicXMLConfigManager::MusicXMLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicXMLConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicXMLConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
{
    if( musics.isEmpty() )
    {
        return;
    }
    //Open wirte file
    if( !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    for(int i=0; i<musics.count(); ++i)
    {
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList", QStringList() << "name" << "index" << "count",
                                  QVariantList() << musics[i].m_itemName << i << musics[i].m_songs.count());
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", QStringList() << "name" << "playCount" << "time",
                                     QVariantList() << song.getMusicName() << song.getMusicPlayCount()
                                                       << song.getMusicTime(), song.getMusicPath());
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicXMLConfigManager::readMusicSongsConfig(MusicSongItems &musics)
{
    QDomNodeList nodes = m_ddom->elementsByTagName("musicList");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        QDomElement element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");
        musics << item;
    }
}

void MusicXMLConfigManager::writeXMLConfig()
{
    int playModeChoiced = M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt();
    int volumeChoiced = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();
    QStringList lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();

    ///////////////////////////////////////////////////////////////////////////
    QPoint widgetPositionChoiced = M_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    QSize widgetSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    int autoPlayChoiced = M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toInt();
    int languageIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    int closeEventChoiced = M_SETTING_PTR->value(MusicSettingManager::CloseEventChoiced).toInt();
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
    QString lrcFgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFgColorChoiced).toString();
    QString lrcBgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcBgColorChoiced).toString();
    int lrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int DLrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt();
    int DLrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt();
    int DLrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt();
    int DLrcFamilyChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt();
    QString DLrcFgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFgColorChoiced).toString();
    QString DLrcBgColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcBgColorChoiced).toString();
    int DLrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt();
    int DLrcWindowTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcWindowTypeChoiced).toInt();
    int DLrcLockedChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcLockedChoiced).toInt();
    QPoint DLrcGeometry = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometryChoiced).toPoint();

    ///////////////////////////////////////////////////////////////////////////
    int enhancedMusicChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    int equalizerEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    QString equalizerValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString();
    int equalizerIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt();
    int enhancedBalanceChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedBalanceChoiced).toInt();
    int enhancedFadeEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnableChoiced).toInt();
    int enhancedFadeInValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValueChoiced).toInt();
    int enhancedFadeOutValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValueChoiced).toInt();
    int enhancedBS2BChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedBS2BChoiced).toInt();
    int enhancedCrossfadeChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfadeChoiced).toInt();
    int enhancedStereoChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedStereoChoiced).toInt();
    int enhancedLADSPAChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPAChoiced).toInt();
    int enhancedSOXChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedSOXChoiced).toInt();

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
    int downloadServerMultiple = M_SETTING_PTR->value(MusicSettingManager::DownloadServerMultipleChoiced).toInt();
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
    writeDomElementText(music, "lastPlayIndex", "value", lastPlayIndexChoiced[0],
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(settings, "geometry", "value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height()));
    writeDomElement(settings, "language", "value", languageIndexChoiced);
    writeDomElement(settings, "autoPlay", "value", autoPlayChoiced);
    writeDomElement(settings, "closeEvent", "value", closeEventChoiced);
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
    writeDomElement(showLrc, "lrcFgColor", "value", lrcFgColorChoiced);
    writeDomElement(showLrc, "lrcBgColor", "value", lrcBgColorChoiced);

    ///////////////////////////////////////////////
    writeDomElement(showDLrc, "showDesktopLrc", "value", showDesktopLrcChoiced);
    writeDomElement(showDLrc, "lrcDColor", "value", DLrcColorChoiced);
    writeDomElement(showDLrc, "lrcDSize", "value", DLrcSizeChoiced);
    writeDomElement(showDLrc, "lrcDFamily", "value", DLrcFamilyChoiced);
    writeDomElement(showDLrc, "lrcDType", "value", DLrcTypeChoiced);
    writeDomElement(showDLrc, "lrcDTransparent", "value", DLrcTransparentChoiced);
    writeDomElement(showDLrc, "lrcDFgColor", "value", DLrcFgColorChoiced);
    writeDomElement(showDLrc, "lrcDBgColor", "value", DLrcBgColorChoiced);

    writeDomElement(showDLrc, "lrcDWindowType", "value", DLrcWindowTypeChoiced);
    writeDomElement(showDLrc, "lrcDLocked", "value", DLrcLockedChoiced);
    writeDomElement(showDLrc, "lrcDGeometry", "value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y()));

    ///////////////////////////////////////////////
    writeDomElement(equalizer, "enhancedMusic", "value", enhancedMusicChoiced);
    writeDomElement(equalizer, "equalizerEnable", "value", equalizerEnableChoiced);
    writeDomElement(equalizer, "equalizerIndex", "value", equalizerIndexChoiced);
    writeDomElement(equalizer, "equalizerValue", "value", equalizerValueChoiced);
    writeDomElement(equalizer, "enhancedBalance", "value", enhancedBalanceChoiced);
    writeDomElement(equalizer, "enhancedFadeEnable", "value", enhancedFadeEnableChoiced);
    writeDomElement(equalizer, "enhancedFadeInValue", "value", enhancedFadeInValueChoiced);
    writeDomElement(equalizer, "enhancedFadeOutValue", "value", enhancedFadeOutValueChoiced);
    writeDomElement(equalizer, "enhancedBS2B", "value", enhancedBS2BChoiced);
    writeDomElement(equalizer, "enhancedCrossfade", "value", enhancedCrossfadeChoiced);
    writeDomElement(equalizer, "enhancedStereo", "value", enhancedStereoChoiced);
    writeDomElement(equalizer, "enhancedLADSPA", "value", enhancedLADSPAChoiced);
    writeDomElement(equalizer, "enhancedSOX", "value", enhancedSOXChoiced);

    ///////////////////////////////////////////////
    writeDomElement(downloads, "downloadMusicPath", "value", downloadMusicPath);
    writeDomElement(downloads, "downloadLrcPath", "value", downloadLrcPath);
    writeDomElement(downloads, "downloadCacheLimit", "value", downloadCacheLimit);
    writeDomElement(downloads, "downloadCacheSize", "value", downloadCacheSize);
    writeDomElement(downloads, "downloadLimit", "value", downloadLimit);
    writeDomElement(downloads, "downloadServer", "value", downloadServer);
    writeDomElement(downloads, "downloadServerMultiple", "value", downloadServerMultiple);
    writeDomElement(downloads, "downloadDLoadLimit", "value", downloadDLoadLimit);
    writeDomElement(downloads, "downloadULoadLimit", "value", downloadULoadLimit);
    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicXMLConfigManager::readSystemLastPlayIndexConfig(QStringList &key) const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("lastPlayIndex");
    if(nodelist.isEmpty())
    {
        key << "0" << "0" << "-1";
        return;
    }

    QDomElement element = nodelist.at(0).toElement();
    key << element.attribute("value") << element.text().split(',');
    if(key.count() != 3)
    {
        key.clear();
        key << "0" << "0" << "-1";
    }
}

QRect MusicXMLConfigManager::readWindowGeometry() const
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("geometry");
    if(nodelist.isEmpty())
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }

    QDomElement element = nodelist.at(0).toElement();
    QStringList lists = element.attribute("value").split(",");
    if(lists.count() == 4)
    {
        return QRect(lists[0].toInt() < 0 ? 0 : lists[0].toInt(),
                     lists[1].toInt() < 0 ? 0 : lists[1].toInt(),
                     lists[2].toInt(), lists[3].toInt());
    }
    else
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }
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

    QString path = readXmlAttributeByTagNameValue("downloadMusicPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDirChoiced,
                     path.isEmpty() ? MUSIC_DIR_FULL : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced,
                     path.isEmpty() ? LRC_DIR_FULL : path);
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSizeChoiced,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerChoiced,
                     readXmlAttributeByTagNameValue("downloadServer").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServerMultipleChoiced,
                     readXmlAttributeByTagNameValue("downloadServerMultiple").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit"));
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadULoadLimit"));

    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced,
                     readXmlAttributeByTagNameValue("fileAssociation"));

}

MusicSongs MusicXMLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    QDomNodeList nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        QDomElement element = nodelist.at(i).toElement();
        songs << MusicSong(element.text(),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
