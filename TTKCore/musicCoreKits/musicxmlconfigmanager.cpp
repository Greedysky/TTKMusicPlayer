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
    writeDomElement(music, "playMode", XmlAttribute("value", playModeChoiced));
    writeDomElement(music, "playVolume", XmlAttribute("value", volumeChoiced));
    writeDomElementText(music, "lastPlayIndex", XmlAttribute("value", lastPlayIndexChoiced[0]),
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(settings, "geometry", XmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height())));
    writeDomElement(settings, "language", XmlAttribute("value", languageIndexChoiced));
    writeDomElement(settings, "autoPlay", XmlAttribute("value", autoPlayChoiced));
    writeDomElement(settings, "closeEvent", XmlAttribute("value", closeEventChoiced));
    writeDomElement(settings, "closeNetwork", XmlAttribute("value", closeNetWorkChoiced));
    writeDomElement(settings, "fileAssociation", XmlAttribute("value", fileAssociationChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(timeSettings, "timeAutoIndex", XmlAttribute("value", timeAutoIndexChoiced));
    writeDomElement(timeSettings, "timeAutoPlay", XmlAttribute("value", timeAutoPlayChoiced));
    writeDomElement(timeSettings, "timeAutoPlayHour", XmlAttribute("value", timeAutoPlayHourChoiced));
    writeDomElement(timeSettings, "timeAutoPlaySecond", XmlAttribute("value", timeAutoPlaySecondChoiced));
    writeDomElement(timeSettings, "timeAutoPlayRepeat", XmlAttribute("value", timeAutoPlayRepeatChoiced));
    writeDomElement(timeSettings, "timeAutoPlayItemIndex", XmlAttribute("value", timeAutoPlayItemIndexChoiced));
    writeDomElement(timeSettings, "timeAutoPlaySongIndex", XmlAttribute("value", timeAutoPlaySongIndexChoiced));
    writeDomElement(timeSettings, "timeAutoStop", XmlAttribute("value", timeAutoStopChoiced));
    writeDomElement(timeSettings, "timeAutoStopHour", XmlAttribute("value", timeAutoStopHourChoiced));
    writeDomElement(timeSettings, "timeAutoStopSecond", XmlAttribute("value", timeAutoStopSecondChoiced));
    writeDomElement(timeSettings, "timeAutoStopRepeat", XmlAttribute("value", timeAutoStopRepeatChoiced));
    writeDomElement(timeSettings, "timeAutoShutdown", XmlAttribute("value", timeAutoShutdownChoiced));
    writeDomElement(timeSettings, "timeAutoShutdownHour", XmlAttribute("value", timeAutoShutdownHourChoiced));
    writeDomElement(timeSettings, "timeAutoShutdownSecond", XmlAttribute("value", timeAutoShutdownSecondChoiced));
    writeDomElement(timeSettings, "timeAutoShutdownRepeat", XmlAttribute("value", timeAutoShutdownRepeatChoiced));

    ///////////////////////////////////////////////////////////////////////////
    writeDomElement(backgroundSkin, "bgTheme", XmlAttribute("value", bgThemeChoiced));
    writeDomElement(backgroundSkin, "bgTransparent", XmlAttribute("value", bgTransparentChoiced));
    writeDomElement(backgroundSkin, "bgListTransparent", XmlAttribute("value", bgListTransparentChoiced));

    ///////////////////////////////////////////////
    writeDomElement(showLrc, "showInlineLrc", XmlAttribute("value", showInlineLrcChoiced));
    writeDomElement(showLrc, "lrcColor", XmlAttribute("value", lrcColorChoiced));
    writeDomElement(showLrc, "lrcSize", XmlAttribute("value", lrcSizeChoiced));
    writeDomElement(showLrc, "lrcFamily", XmlAttribute("value", lrcFamilyChoiced));
    writeDomElement(showLrc, "lrcType", XmlAttribute("value", lrcTypeChoiced));
    writeDomElement(showLrc, "lrcTransparent", XmlAttribute("value", lrcTransparentChoiced));
    writeDomElement(showLrc, "lrcFgColor", XmlAttribute("value", lrcFgColorChoiced));
    writeDomElement(showLrc, "lrcBgColor", XmlAttribute("value", lrcBgColorChoiced));

    ///////////////////////////////////////////////
    writeDomElement(showDLrc, "showDesktopLrc", XmlAttribute("value", showDesktopLrcChoiced));
    writeDomElement(showDLrc, "lrcDColor", XmlAttribute("value", DLrcColorChoiced));
    writeDomElement(showDLrc, "lrcDSize", XmlAttribute("value", DLrcSizeChoiced));
    writeDomElement(showDLrc, "lrcDFamily", XmlAttribute("value", DLrcFamilyChoiced));
    writeDomElement(showDLrc, "lrcDType", XmlAttribute("value", DLrcTypeChoiced));
    writeDomElement(showDLrc, "lrcDTransparent", XmlAttribute("value", DLrcTransparentChoiced));
    writeDomElement(showDLrc, "lrcDFgColor", XmlAttribute("value", DLrcFgColorChoiced));
    writeDomElement(showDLrc, "lrcDBgColor", XmlAttribute("value", DLrcBgColorChoiced));

    writeDomElement(showDLrc, "lrcDWindowType", XmlAttribute("value", DLrcWindowTypeChoiced));
    writeDomElement(showDLrc, "lrcDSingleLineType", XmlAttribute("value", DLrcSingleLineTypeChoiced));
    writeDomElement(showDLrc, "lrcDLocked", XmlAttribute("value", DLrcLockedChoiced));
    writeDomElement(showDLrc, "lrcDGeometry", XmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));

    ///////////////////////////////////////////////
    writeDomElement(equalizer, "enhancedMusic", XmlAttribute("value", enhancedMusicChoiced));
    writeDomElement(equalizer, "equalizerEnable", XmlAttribute("value", equalizerEnableChoiced));
    writeDomElement(equalizer, "equalizerIndex", XmlAttribute("value", equalizerIndexChoiced));
    writeDomElement(equalizer, "equalizerValue", XmlAttribute("value", equalizerValueChoiced));
    writeDomElement(equalizer, "enhancedBalance", XmlAttribute("value", enhancedBalanceChoiced));
    writeDomElement(equalizer, "enhancedFadeEnable", XmlAttribute("value", enhancedFadeEnableChoiced));
    writeDomElement(equalizer, "enhancedFadeInValue", XmlAttribute("value", enhancedFadeInValueChoiced));
    writeDomElement(equalizer, "enhancedFadeOutValue", XmlAttribute("value", enhancedFadeOutValueChoiced));
    writeDomElement(equalizer, "enhancedBS2B", XmlAttribute("value", enhancedBS2BChoiced));
    writeDomElement(equalizer, "enhancedCrossfade", XmlAttribute("value", enhancedCrossfadeChoiced));
    writeDomElement(equalizer, "enhancedStereo", XmlAttribute("value", enhancedStereoChoiced));
    writeDomElement(equalizer, "enhancedLADSPA", XmlAttribute("value", enhancedLADSPAChoiced));
    writeDomElement(equalizer, "enhancedSOX", XmlAttribute("value", enhancedSOXChoiced));

    ///////////////////////////////////////////////
    writeDomElement(downloads, "downloadMusicPath", XmlAttribute("value", downloadMusicPath));
    writeDomElement(downloads, "downloadLrcPath", XmlAttribute("value", downloadLrcPath));
    writeDomElement(downloads, "downloadCacheLimit", XmlAttribute("value", downloadCacheLimit));
    writeDomElement(downloads, "downloadCacheSize", XmlAttribute("value", downloadCacheSize));
    writeDomElement(downloads, "downloadLimit", XmlAttribute("value", downloadLimit));
    writeDomElement(downloads, "downloadServer", XmlAttribute("value", downloadServer));
    writeDomElement(downloads, "downloadDLoadLimit", XmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloads, "downloadULoadLimit", XmlAttribute("value", downloadULoadLimit));
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
