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
        const MusicSongItem item = musics[i];
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList",
                              XmlAttributes() << XmlAttribute("name", item.m_itemName) << XmlAttribute("index", i)
                                              << XmlAttribute("count", item.m_songs.count()) << XmlAttribute("sortIndex", item.m_sort.m_index)
                                              << XmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", XmlAttributes() << XmlAttribute("name", song.getMusicName())
                                     << XmlAttribute("playCount", song.getMusicPlayCount())
                                     << XmlAttribute("time", song.getMusicTime()), song.getMusicPath());
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
    QDomElement musicDom = writeDom(musicPlayerDom, "music");
    QDomElement settingsDom = writeDom(musicPlayerDom, "settings");
    QDomElement backgroundSkinDom = writeDom(musicPlayerDom, "backgroundSkin");
    QDomElement hotkeyDom = writeDom(musicPlayerDom, "hotkey");
    QDomElement showLrcDom = writeDom(musicPlayerDom, "inlineLrc");
    QDomElement showDLrcDom = writeDom(musicPlayerDom, "desktopLrc");
    QDomElement equalizerDom = writeDom(musicPlayerDom, "equalizer");
    QDomElement timeSettingsDom = writeDom(musicPlayerDom, "timeSettings");
    QDomElement downloadsDom = writeDom(musicPlayerDom, "downloads");
    //Class B
    writeDomElement(musicDom, "playMode", XmlAttribute("value", playModeChoiced));
    writeDomElement(musicDom, "playVolume", XmlAttribute("value", volumeChoiced));
    writeDomElementText(musicDom, "lastPlayIndex", XmlAttribute("value", lastPlayIndexChoiced[0]),
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(settingsDom, "geometry", XmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height())));
    writeDomElement(settingsDom, "language", XmlAttribute("value", languageIndexChoiced));
    writeDomElement(settingsDom, "autoPlay", XmlAttribute("value", autoPlayChoiced));
    writeDomElement(settingsDom, "closeEvent", XmlAttribute("value", closeEventChoiced));
    writeDomElement(settingsDom, "closeNetwork", XmlAttribute("value", closeNetWorkChoiced));
    writeDomElement(settingsDom, "fileAssociation", XmlAttribute("value", fileAssociationChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(backgroundSkinDom, "bgTheme", XmlAttribute("value", bgThemeChoiced));
    writeDomElement(backgroundSkinDom, "bgTransparent", XmlAttribute("value", bgTransparentChoiced));
    writeDomElement(backgroundSkinDom, "bgListTransparent", XmlAttribute("value", bgListTransparentChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(hotkeyDom, "hotkeyEnable", XmlAttribute("value", hotkeyEnableChoiced));
    writeDomElement(hotkeyDom, "hotkeyString", XmlAttribute("value", hotkeyStringChoiced));

    ///////////////////////////////////////////////
    writeDomElement(showLrcDom, "showInlineLrc", XmlAttribute("value", showInlineLrcChoiced));
    writeDomElement(showLrcDom, "lrcColor", XmlAttribute("value", lrcColorChoiced));
    writeDomElement(showLrcDom, "lrcSize", XmlAttribute("value", lrcSizeChoiced));
    writeDomElement(showLrcDom, "lrcFamily", XmlAttribute("value", lrcFamilyChoiced));
    writeDomElement(showLrcDom, "lrcType", XmlAttribute("value", lrcTypeChoiced));
    writeDomElement(showLrcDom, "lrcTransparent", XmlAttribute("value", lrcTransparentChoiced));
    writeDomElement(showLrcDom, "lrcFgColor", XmlAttribute("value", lrcFgColorChoiced));
    writeDomElement(showLrcDom, "lrcBgColor", XmlAttribute("value", lrcBgColorChoiced));

    ///////////////////////////////////////////////
    writeDomElement(showDLrcDom, "showDesktopLrc", XmlAttribute("value", showDesktopLrcChoiced));
    writeDomElement(showDLrcDom, "lrcDColor", XmlAttribute("value", DLrcColorChoiced));
    writeDomElement(showDLrcDom, "lrcDSize", XmlAttribute("value", DLrcSizeChoiced));
    writeDomElement(showDLrcDom, "lrcDFamily", XmlAttribute("value", DLrcFamilyChoiced));
    writeDomElement(showDLrcDom, "lrcDType", XmlAttribute("value", DLrcTypeChoiced));
    writeDomElement(showDLrcDom, "lrcDTransparent", XmlAttribute("value", DLrcTransparentChoiced));
    writeDomElement(showDLrcDom, "lrcDFgColor", XmlAttribute("value", DLrcFgColorChoiced));
    writeDomElement(showDLrcDom, "lrcDBgColor", XmlAttribute("value", DLrcBgColorChoiced));
    writeDomElement(showDLrcDom, "lrcDWindowType", XmlAttribute("value", DLrcWindowTypeChoiced));
    writeDomElement(showDLrcDom, "lrcDSingleLineType", XmlAttribute("value", DLrcSingleLineTypeChoiced));
    writeDomElement(showDLrcDom, "lrcDLocked", XmlAttribute("value", DLrcLockedChoiced));
    writeDomElement(showDLrcDom, "lrcDGeometry", XmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));

    ///////////////////////////////////////////////
    writeDomElement(equalizerDom, "enhancedMusic", XmlAttribute("value", enhancedMusicChoiced));
    writeDomElement(equalizerDom, "equalizerEnable", XmlAttribute("value", equalizerEnableChoiced));
    writeDomElement(equalizerDom, "equalizerIndex", XmlAttribute("value", equalizerIndexChoiced));
    writeDomElement(equalizerDom, "equalizerValue", XmlAttribute("value", equalizerValueChoiced));
    writeDomElement(equalizerDom, "enhancedBalance", XmlAttribute("value", enhancedBalanceChoiced));
    writeDomElement(equalizerDom, "enhancedFadeEnable", XmlAttribute("value", enhancedFadeEnableChoiced));
    writeDomElement(equalizerDom, "enhancedFadeInValue", XmlAttribute("value", enhancedFadeInValueChoiced));
    writeDomElement(equalizerDom, "enhancedFadeOutValue", XmlAttribute("value", enhancedFadeOutValueChoiced));
    writeDomElement(equalizerDom, "enhancedBS2B", XmlAttribute("value", enhancedBS2BChoiced));
    writeDomElement(equalizerDom, "enhancedCrossfade", XmlAttribute("value", enhancedCrossfadeChoiced));
    writeDomElement(equalizerDom, "enhancedStereo", XmlAttribute("value", enhancedStereoChoiced));
    writeDomElement(equalizerDom, "enhancedLADSPA", XmlAttribute("value", enhancedLADSPAChoiced));
    writeDomElement(equalizerDom, "enhancedSOX", XmlAttribute("value", enhancedSOXChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(timeSettingsDom, "timeAutoIndex", XmlAttribute("value", timeAutoIndexChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlay", XmlAttribute("value", timeAutoPlayChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlayHour", XmlAttribute("value", timeAutoPlayHourChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlaySecond", XmlAttribute("value", timeAutoPlaySecondChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlayRepeat", XmlAttribute("value", timeAutoPlayRepeatChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlayItemIndex", XmlAttribute("value", timeAutoPlayItemIndexChoiced));
    writeDomElement(timeSettingsDom, "timeAutoPlaySongIndex", XmlAttribute("value", timeAutoPlaySongIndexChoiced));
    writeDomElement(timeSettingsDom, "timeAutoStop", XmlAttribute("value", timeAutoStopChoiced));
    writeDomElement(timeSettingsDom, "timeAutoStopHour", XmlAttribute("value", timeAutoStopHourChoiced));
    writeDomElement(timeSettingsDom, "timeAutoStopSecond", XmlAttribute("value", timeAutoStopSecondChoiced));
    writeDomElement(timeSettingsDom, "timeAutoStopRepeat", XmlAttribute("value", timeAutoStopRepeatChoiced));
    writeDomElement(timeSettingsDom, "timeAutoShutdown", XmlAttribute("value", timeAutoShutdownChoiced));
    writeDomElement(timeSettingsDom, "timeAutoShutdownHour", XmlAttribute("value", timeAutoShutdownHourChoiced));
    writeDomElement(timeSettingsDom, "timeAutoShutdownSecond", XmlAttribute("value", timeAutoShutdownSecondChoiced));
    writeDomElement(timeSettingsDom, "timeAutoShutdownRepeat", XmlAttribute("value", timeAutoShutdownRepeatChoiced));

    ///////////////////////////////////////////////
    writeDomElement(downloadsDom, "downloadMusicPath", XmlAttribute("value", downloadMusicPath));
    writeDomElement(downloadsDom, "downloadLrcPath", XmlAttribute("value", downloadLrcPath));
    writeDomElement(downloadsDom, "downloadCacheLimit", XmlAttribute("value", downloadCacheLimit));
    writeDomElement(downloadsDom, "downloadCacheSize", XmlAttribute("value", downloadCacheSize));
    writeDomElement(downloadsDom, "downloadLimit", XmlAttribute("value", downloadLimit));
    writeDomElement(downloadsDom, "downloadServer", XmlAttribute("value", downloadServer));
    writeDomElement(downloadsDom, "downloadDLoadLimit", XmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloadsDom, "downloadULoadLimit", XmlAttribute("value", downloadULoadLimit));

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
