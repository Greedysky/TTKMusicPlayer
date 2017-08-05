#include "musicxmlconfigmanager.h"
#include "musicsettingmanager.h"
#include "musiccoreutils.h"

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
        const MusicSongItem item = musics[i];
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList",
                              MusicXmlAttributes() << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                              << MusicXmlAttribute("count", item.m_songs.count()) << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", MusicXmlAttributes() << MusicXmlAttribute("name", song.getMusicName())
                                     << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                     << MusicXmlAttribute("time", song.getMusicTime()), song.getMusicPath());
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
        QString string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
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
    int windowConciseChoiced = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toInt();
    int remoteWidgetModeChoiced = M_SETTING_PTR->value(MusicSettingManager::RemoteWidgetModeChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    int otherBgLosslessChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherBgLosslessChoiced).toInt();
    int otherUpdateChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherUpdateChoiced).toInt();
    int otherSearchChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherSearchChoiced).toInt();
    int otherAlbumChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherAlbumChoiced).toInt();
    int otherInfoChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherInfoChoiced).toInt();
    int otherSideByChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toInt();

    ///////////////////////////////////////////////////////////////////////////
    QString bgThemeChoiced = M_SETTING_PTR->value(MusicSettingManager::BgThemeChoiced).toString();
    QString bgTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BgTransparentChoiced).toString();
    QString bgListTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BgListTransparentChoiced).toString();

    ///////////////////////////////////////////////////////////////////////////
    int hotkeyEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::HotkeyEnableChoiced).toInt();
    QString hotkeyStringChoiced = M_SETTING_PTR->value(MusicSettingManager::HotkeyStringChoiced).toString();

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
    int DLrcSingleLineTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineTypeChoiced).toInt();
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
    QDomElement musicPlayerDom = createRoot("TTKMusicPlayer");
    //Class A
    QDomElement musicSettingDom = writeDom(musicPlayerDom, "musicSetting");
    QDomElement plusSettingDom = writeDom(musicPlayerDom, "plusSetting");
    QDomElement otherSettingDom = writeDom(musicPlayerDom, "otherSetting");
    QDomElement backgroundSettingDom = writeDom(musicPlayerDom, "backgroundSetting");
    QDomElement hotkeySettingDom = writeDom(musicPlayerDom, "hotkeySetting");
    QDomElement inlineLrcSettingDom = writeDom(musicPlayerDom, "inlineLrcSetting");
    QDomElement desktopLrcSettingDom = writeDom(musicPlayerDom, "desktopLrcSetting");
    QDomElement equalizerSettingDom = writeDom(musicPlayerDom, "equalizerSetting");
    QDomElement timeSettingDom = writeDom(musicPlayerDom, "timeSetting");
    QDomElement downloadSettingDom = writeDom(musicPlayerDom, "downloadSetting");
    //Class B
    writeDomElement(musicSettingDom, "playMode", MusicXmlAttribute("value", playModeChoiced));
    writeDomElement(musicSettingDom, "playVolume", MusicXmlAttribute("value", volumeChoiced));
    writeDomElementText(musicSettingDom, "lastPlayIndex", MusicXmlAttribute("value", lastPlayIndexChoiced[0]),
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(plusSettingDom, "geometry", MusicXmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height())));
    writeDomElement(plusSettingDom, "language", MusicXmlAttribute("value", languageIndexChoiced));
    writeDomElement(plusSettingDom, "autoPlay", MusicXmlAttribute("value", autoPlayChoiced));
    writeDomElement(plusSettingDom, "closeEvent", MusicXmlAttribute("value", closeEventChoiced));
    writeDomElement(plusSettingDom, "closeNetwork", MusicXmlAttribute("value", closeNetWorkChoiced));
    writeDomElement(plusSettingDom, "fileAssociation", MusicXmlAttribute("value", fileAssociationChoiced));
    writeDomElement(plusSettingDom, "windowConcise", MusicXmlAttribute("value", windowConciseChoiced));
    writeDomElement(plusSettingDom, "remoteWidgetMode", MusicXmlAttribute("value", remoteWidgetModeChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(otherSettingDom, "otherBgLossless", MusicXmlAttribute("value", otherBgLosslessChoiced));
    writeDomElement(otherSettingDom, "otherUpdate", MusicXmlAttribute("value", otherUpdateChoiced));
    writeDomElement(otherSettingDom, "otherSearch", MusicXmlAttribute("value", otherSearchChoiced));
    writeDomElement(otherSettingDom, "otherAlbum", MusicXmlAttribute("value", otherAlbumChoiced));
    writeDomElement(otherSettingDom, "otherInfo", MusicXmlAttribute("value", otherInfoChoiced));
    writeDomElement(otherSettingDom, "otherSideBy", MusicXmlAttribute("value", otherSideByChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(backgroundSettingDom, "bgTheme", MusicXmlAttribute("value", bgThemeChoiced));
    writeDomElement(backgroundSettingDom, "bgTransparent", MusicXmlAttribute("value", bgTransparentChoiced));
    writeDomElement(backgroundSettingDom, "bgListTransparent", MusicXmlAttribute("value", bgListTransparentChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(hotkeySettingDom, "hotkeyEnable", MusicXmlAttribute("value", hotkeyEnableChoiced));
    writeDomElement(hotkeySettingDom, "hotkeyString", MusicXmlAttribute("value", hotkeyStringChoiced));

    ///////////////////////////////////////////////
    writeDomElement(inlineLrcSettingDom, "showInlineLrc", MusicXmlAttribute("value", showInlineLrcChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcColor", MusicXmlAttribute("value", lrcColorChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcSize", MusicXmlAttribute("value", lrcSizeChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcFamily", MusicXmlAttribute("value", lrcFamilyChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcType", MusicXmlAttribute("value", lrcTypeChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcTransparent", MusicXmlAttribute("value", lrcTransparentChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcFgColor", MusicXmlAttribute("value", lrcFgColorChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcBgColor", MusicXmlAttribute("value", lrcBgColorChoiced));

    ///////////////////////////////////////////////
    writeDomElement(desktopLrcSettingDom, "showDesktopLrc", MusicXmlAttribute("value", showDesktopLrcChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDColor", MusicXmlAttribute("value", DLrcColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDSize", MusicXmlAttribute("value", DLrcSizeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDFamily", MusicXmlAttribute("value", DLrcFamilyChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDType", MusicXmlAttribute("value", DLrcTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDTransparent", MusicXmlAttribute("value", DLrcTransparentChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDFgColor", MusicXmlAttribute("value", DLrcFgColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDBgColor", MusicXmlAttribute("value", DLrcBgColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDWindowType", MusicXmlAttribute("value", DLrcWindowTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDSingleLineType", MusicXmlAttribute("value", DLrcSingleLineTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDLocked", MusicXmlAttribute("value", DLrcLockedChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDGeometry", MusicXmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));

    ///////////////////////////////////////////////
    writeDomElement(equalizerSettingDom, "enhancedMusic", MusicXmlAttribute("value", enhancedMusicChoiced));
    writeDomElement(equalizerSettingDom, "equalizerEnable", MusicXmlAttribute("value", equalizerEnableChoiced));
    writeDomElement(equalizerSettingDom, "equalizerIndex", MusicXmlAttribute("value", equalizerIndexChoiced));
    writeDomElement(equalizerSettingDom, "equalizerValue", MusicXmlAttribute("value", equalizerValueChoiced));
    writeDomElement(equalizerSettingDom, "enhancedBalance", MusicXmlAttribute("value", enhancedBalanceChoiced));
    writeDomElement(equalizerSettingDom, "enhancedFadeEnable", MusicXmlAttribute("value", enhancedFadeEnableChoiced));
    writeDomElement(equalizerSettingDom, "enhancedFadeInValue", MusicXmlAttribute("value", enhancedFadeInValueChoiced));
    writeDomElement(equalizerSettingDom, "enhancedFadeOutValue", MusicXmlAttribute("value", enhancedFadeOutValueChoiced));
    writeDomElement(equalizerSettingDom, "enhancedBS2B", MusicXmlAttribute("value", enhancedBS2BChoiced));
    writeDomElement(equalizerSettingDom, "enhancedCrossfade", MusicXmlAttribute("value", enhancedCrossfadeChoiced));
    writeDomElement(equalizerSettingDom, "enhancedStereo", MusicXmlAttribute("value", enhancedStereoChoiced));
    writeDomElement(equalizerSettingDom, "enhancedLADSPA", MusicXmlAttribute("value", enhancedLADSPAChoiced));
    writeDomElement(equalizerSettingDom, "enhancedSOX", MusicXmlAttribute("value", enhancedSOXChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(timeSettingDom, "timeAutoIndex", MusicXmlAttribute("value", timeAutoIndexChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlay", MusicXmlAttribute("value", timeAutoPlayChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlayHour", MusicXmlAttribute("value", timeAutoPlayHourChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlaySecond", MusicXmlAttribute("value", timeAutoPlaySecondChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlayRepeat", MusicXmlAttribute("value", timeAutoPlayRepeatChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlayItemIndex", MusicXmlAttribute("value", timeAutoPlayItemIndexChoiced));
    writeDomElement(timeSettingDom, "timeAutoPlaySongIndex", MusicXmlAttribute("value", timeAutoPlaySongIndexChoiced));
    writeDomElement(timeSettingDom, "timeAutoStop", MusicXmlAttribute("value", timeAutoStopChoiced));
    writeDomElement(timeSettingDom, "timeAutoStopHour", MusicXmlAttribute("value", timeAutoStopHourChoiced));
    writeDomElement(timeSettingDom, "timeAutoStopSecond", MusicXmlAttribute("value", timeAutoStopSecondChoiced));
    writeDomElement(timeSettingDom, "timeAutoStopRepeat", MusicXmlAttribute("value", timeAutoStopRepeatChoiced));
    writeDomElement(timeSettingDom, "timeAutoShutdown", MusicXmlAttribute("value", timeAutoShutdownChoiced));
    writeDomElement(timeSettingDom, "timeAutoShutdownHour", MusicXmlAttribute("value", timeAutoShutdownHourChoiced));
    writeDomElement(timeSettingDom, "timeAutoShutdownSecond", MusicXmlAttribute("value", timeAutoShutdownSecondChoiced));
    writeDomElement(timeSettingDom, "timeAutoShutdownRepeat", MusicXmlAttribute("value", timeAutoShutdownRepeatChoiced));

    ///////////////////////////////////////////////
    writeDomElement(downloadSettingDom, "downloadMusicPath", MusicXmlAttribute("value", downloadMusicPath));
    writeDomElement(downloadSettingDom, "downloadLrcPath", MusicXmlAttribute("value", downloadLrcPath));
    writeDomElement(downloadSettingDom, "downloadCacheLimit", MusicXmlAttribute("value", downloadCacheLimit));
    writeDomElement(downloadSettingDom, "downloadCacheSize", MusicXmlAttribute("value", downloadCacheSize));
    writeDomElement(downloadSettingDom, "downloadLimit", MusicXmlAttribute("value", downloadLimit));
    writeDomElement(downloadSettingDom, "downloadServer", MusicXmlAttribute("value", downloadServer));
    writeDomElement(downloadSettingDom, "downloadDLoadLimit", MusicXmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloadSettingDom, "downloadULoadLimit", MusicXmlAttribute("value", downloadULoadLimit));

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
                     (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::Core::musicPrefix() : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced,
                     (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::Core::lrcPrefix() : path);
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

    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnableChoiced,
                     readXmlAttributeByTagNameValue("hotkeyEnable"));
    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyStringChoiced,
                     readXmlAttributeByTagNameValue("hotkeyString"));
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
