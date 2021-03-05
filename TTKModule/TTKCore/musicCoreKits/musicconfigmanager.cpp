#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"

#include <QRect>

MusicConfigManager::MusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicConfigManager::readSysConfigData() const
{
    G_SETTING_PTR->setValue(MusicSettingManager::PlayMode,
                     readXmlAttributeByTagNameValue("playMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Volume,
                     readXmlAttributeByTagNameValue("playVolume").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndex,
                     readXmlAttributeByTagNameValue("language").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::AutoPlay,
                     readXmlAttributeByTagNameValue("autoPlay").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseEvent,
                     readXmlAttributeByTagNameValue("closeEvent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseNetWork,
                     readXmlAttributeByTagNameValue("closeNetwork").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociation,
                     readXmlAttributeByTagNameValue("fileAssociation").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConcise,
                     readXmlAttributeByTagNameValue("windowConcise").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode,
                     readXmlAttributeByTagNameValue("remoteWidgetMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode,
                     readXmlAttributeByTagNameValue("windowQuitMode").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::OtherBackgroundLossless,
                     readXmlAttributeByTagNameValue("otherBackgroundLossless").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdate,
                     readXmlAttributeByTagNameValue("otherCheckUpdate").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSearch,
                     readXmlAttributeByTagNameValue("otherSearch").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCover,
                     readXmlAttributeByTagNameValue("otherUseAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseInfo,
                     readXmlAttributeByTagNameValue("otherUseInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover,
                     readXmlAttributeByTagNameValue("otherWriteAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteInfo,
                     readXmlAttributeByTagNameValue("otherWriteInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideBy,
                     readXmlAttributeByTagNameValue("otherSideBy").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSongFormat,
                     readXmlAttributeByTagNameValue("otherSongFormat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode,
                     readXmlAttributeByTagNameValue("otherLrcKTVMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable,
                     readXmlAttributeByTagNameValue("otherScreenSaverEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime,
                     readXmlAttributeByTagNameValue("otherScreenSaverTime").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex,
                     readXmlAttributeByTagNameValue("otherScreenSaverIndex"));


    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable,
                     readXmlAttributeByTagNameValue("rippleSpectrumEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor,
                     readXmlAttributeByTagNameValue("rippleSpectrumColor"));


    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTheme,
                     readXmlAttributeByTagNameValue("backgroundTheme"));
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent,
                     readXmlAttributeByTagNameValue("backgroundTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable,
                     readXmlAttributeByTagNameValue("backgroundTransparentEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent,
                     readXmlAttributeByTagNameValue("backgroundListTransparent").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable,
                     readXmlAttributeByTagNameValue("hotkeyEnable"));
    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyString,
                     readXmlAttributeByTagNameValue("hotkeyString"));


    G_SETTING_PTR->setValue(MusicSettingManager::ShowInteriorLrc,
                     readXmlAttributeByTagNameValue("showInteriorLrc").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColor,
                     readXmlAttributeByTagNameValue("lrcColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcSize,
                     readXmlAttributeByTagNameValue("lrcSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFamily,
                     readXmlAttributeByTagNameValue("lrcFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColor,
                     readXmlAttributeByTagNameValue("lrcFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColor,
                     readXmlAttributeByTagNameValue("lrcBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcType,
                     readXmlAttributeByTagNameValue("lrcType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColorTrans,
                     readXmlAttributeByTagNameValue("lrcTransparent").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc,
                     readXmlAttributeByTagNameValue("showDesktopLrc").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColor,
                     readXmlAttributeByTagNameValue("lrcDColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSize,
                     readXmlAttributeByTagNameValue("lrcDSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFamily,
                     readXmlAttributeByTagNameValue("lrcDFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColor,
                     readXmlAttributeByTagNameValue("lrcDFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColor,
                     readXmlAttributeByTagNameValue("lrcDBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcType,
                     readXmlAttributeByTagNameValue("lrcDType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTrans,
                     readXmlAttributeByTagNameValue("lrcDTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowType,
                     readXmlAttributeByTagNameValue("lrcDWindowType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineType,
                     readXmlAttributeByTagNameValue("lrcDSingleLineType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcLocked,
                     readXmlAttributeByTagNameValue("lrcDLocked").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusic,
                     readXmlAttributeByTagNameValue("enhancedMusic").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndex,
                     readXmlAttributeByTagNameValue("equalizerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable,
                     readXmlAttributeByTagNameValue("equalizerEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerValue,
                     readXmlAttributeByTagNameValue("equalizerValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable,
                     readXmlAttributeByTagNameValue("enhancedFadeEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValue,
                     readXmlAttributeByTagNameValue("enhancedFadeInValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValue,
                     readXmlAttributeByTagNameValue("enhancedFadeOutValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedBS2B,
                     readXmlAttributeByTagNameValue("enhancedBS2B").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedCrossfade,
                     readXmlAttributeByTagNameValue("enhancedCrossfade").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedStereo,
                     readXmlAttributeByTagNameValue("enhancedStereo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedLADSPA,
                     readXmlAttributeByTagNameValue("enhancedLADSPA").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedSOX,
                     readXmlAttributeByTagNameValue("enhancedSOX").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedSRC,
                     readXmlAttributeByTagNameValue("enhancedSRC").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMonoToStereo,
                     readXmlAttributeByTagNameValue("enhancedMonoToStereo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMono,
                     readXmlAttributeByTagNameValue("enhancedMono").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndex,
                     readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlay,
                     readXmlAttributeByTagNameValue("timeAutoPlay").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayHour,
                     readXmlAttributeByTagNameValue("timeAutoPlayHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySecond,
                     readXmlAttributeByTagNameValue("timeAutoPlaySecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayRepeat,
                     readXmlAttributeByTagNameValue("timeAutoPlayRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayItemIndex,
                     readXmlAttributeByTagNameValue("timeAutoPlayItemIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySongIndex,
                     readXmlAttributeByTagNameValue("timeAutoPlaySongIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStop,
                     readXmlAttributeByTagNameValue("timeAutoStop").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHour,
                     readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecond,
                     readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeat,
                     readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdown,
                     readXmlAttributeByTagNameValue("timeAutoShutdown").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHour,
                     readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecond,
                     readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeat,
                     readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());


    QString path = readXmlAttributeByTagNameValue("downloadMusicPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDir,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::musicPrefix() : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDir,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::lrcPrefix() : path);
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimit,
                     readXmlAttributeByTagNameValue("downloadCacheLimit").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimit,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServer,
                     readXmlAttributeByTagNameValue("downloadServer").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimit,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit"));
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimit,
                     readXmlAttributeByTagNameValue("downloadULoadLimit"));
}

void MusicConfigManager::writeSysConfigData()
{
    const int playMode = G_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt();
    const int volume = G_SETTING_PTR->value(MusicSettingManager::Volume).toInt();
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();

    //
    const QPoint &widgetPosition = G_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    const QSize &widgetSize = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    const int autoPlay = G_SETTING_PTR->value(MusicSettingManager::AutoPlay).toInt();
    const int languageIndex = G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    const int closeEvent = G_SETTING_PTR->value(MusicSettingManager::CloseEvent).toInt();
    const int closeNetWork = G_SETTING_PTR->value(MusicSettingManager::CloseNetWork).toInt();
    const int fileAssociation = G_SETTING_PTR->value(MusicSettingManager::FileAssociation).toInt();
    const int windowConcise = G_SETTING_PTR->value(MusicSettingManager::WindowConcise).toInt();
    const int remoteWidgetMode = G_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    const int windowQuitMode = G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toInt();

    //
    const int otherBackgroundLossless = G_SETTING_PTR->value(MusicSettingManager::OtherBackgroundLossless).toInt();
    const int otherCheckUpdate = G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdate).toInt();
    const int otherSearch = G_SETTING_PTR->value(MusicSettingManager::OtherSearch).toInt();
    const int otherUseAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toInt();
    const int otherUseInfo = G_SETTING_PTR->value(MusicSettingManager::OtherUseInfo).toInt();
    const int otherWriteAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toInt();
    const int otherWriteInfo = G_SETTING_PTR->value(MusicSettingManager::OtherWriteInfo).toInt();
    const int otherSideBy = G_SETTING_PTR->value(MusicSettingManager::OtherSideBy).toInt();
    const int otherSongFormat = G_SETTING_PTR->value(MusicSettingManager::OtherSongFormat).toInt();
    const int otherLrcKTVMode = G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toInt();
    const int otherScreenSaverEnable = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toInt();
    const int otherScreenSaverTime = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    const QString &otherScreenSaverIndex = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();

    //
    const int rippleSpectrumEnable = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toInt();
    const QString &rippleSpectrumColor = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();

    //
    const QString &backgroundTheme = G_SETTING_PTR->value(MusicSettingManager::BackgroundTheme).toString();
    const int backgroundTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
    const int backgroundListTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
    const int backgroundTransparentEnable = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnable).toInt();

    //
    const int hotkeyEnable = G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toInt();
    const QString &hotkeyString = G_SETTING_PTR->value(MusicSettingManager::HotkeyString).toString();

    //
    const int showInteriorLrc = G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toInt();
    const int showDesktopLrc = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt();
    const int lrcColor = G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt();
    const int lrcSize = G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
    const int lrcType = G_SETTING_PTR->value(MusicSettingManager::LrcType).toInt();
    const int lrcFamily = G_SETTING_PTR->value(MusicSettingManager::LrcFamily).toInt();
    const QString &lrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColor).toString();
    const QString &lrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColor).toString();
    const int lrcTransparent = G_SETTING_PTR->value(MusicSettingManager::LrcColorTrans).toInt();

    //
    const int DLrcColor = G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt();
    const int DLrcSize = G_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt();
    const int DLrcType = G_SETTING_PTR->value(MusicSettingManager::DLrcType).toInt();
    const int DLrcFamily = G_SETTING_PTR->value(MusicSettingManager::DLrcFamily).toInt();
    const QString &DLrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColor).toString();
    const QString &DLrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColor).toString();
    const int DLrcTransparent = G_SETTING_PTR->value(MusicSettingManager::DLrcColorTrans).toInt();
    const int DLrcWindowType = G_SETTING_PTR->value(MusicSettingManager::DLrcWindowType).toInt();
    const int DLrcSingleLineType = G_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineType).toInt();
    const int DLrcLocked = G_SETTING_PTR->value(MusicSettingManager::DLrcLocked).toInt();
    const QPoint &DLrcGeometry = G_SETTING_PTR->value(MusicSettingManager::DLrcGeometry).toPoint();

    //
    const int enhancedMusic = G_SETTING_PTR->value(MusicSettingManager::EnhancedMusic).toInt();
    const int equalizerEnable = G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt();
    const QString &equalizerValue = G_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString();
    const int equalizerIndex = G_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt();
    const int enhancedFadeEnable = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnable).toInt();
    const int enhancedFadeInValue = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValue).toInt();
    const int enhancedFadeOutValue = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValue).toInt();
    const int enhancedBS2B = G_SETTING_PTR->value(MusicSettingManager::EnhancedBS2B).toInt();
    const int enhancedCrossfade = G_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfade).toInt();
    const int enhancedStereo = G_SETTING_PTR->value(MusicSettingManager::EnhancedStereo).toInt();
    const int enhancedLADSPA = G_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPA).toInt();
    const int enhancedSOX = G_SETTING_PTR->value(MusicSettingManager::EnhancedSOX).toInt();
    const int enhancedSRC = G_SETTING_PTR->value(MusicSettingManager::EnhancedSRC).toInt();
    const int enhancedMonoToStereo = G_SETTING_PTR->value(MusicSettingManager::EnhancedMonoToStereo).toInt();
    const int enhancedMono = G_SETTING_PTR->value(MusicSettingManager::EnhancedMono).toInt();

    //
    const int timeAutoIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoIndex).toInt();
    const int timeAutoPlay = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlay).toInt();
    const int timeAutoPlayHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHour).toInt();
    const int timeAutoPlaySecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecond).toInt();
    const int timeAutoPlayRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeat).toInt();
    const int timeAutoPlayItemIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndex).toInt();
    const int timeAutoPlaySongIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndex).toInt();
    const int timeAutoStop = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStop).toInt();
    const int timeAutoStopHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHour).toInt();
    const int timeAutoStopSecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecond).toInt();
    const int timeAutoStopRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeat).toInt();
    const int timeAutoShutdown = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdown).toInt();
    const int timeAutoShutdownHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHour).toInt();
    const int timeAutoShutdownSecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecond).toInt();
    const int timeAutoShutdownRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeat).toInt();
    //

    const QString &downloadMusicPath = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString();
    const QString &downloadLrcPath = G_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDir).toString();
    const int downloadCacheLimit = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimit).toInt();
    const int downloadCacheSize = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt();
    const int downloadLimit = G_SETTING_PTR->value(MusicSettingManager::DownloadLimit).toInt();
    const int downloadServer = G_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    const QString &downloadDLoadLimit = G_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimit).toString();
    const QString &downloadULoadLimit = G_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimit).toString();

    //
    if(!writeConfig(COFIGPATH_FULL))
    {
        return;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayerDom = createRoot(APP_NAME);

    QDomElement musicSettingDom = writeDomNode(musicPlayerDom, "musicSetting");
    QDomElement plusSettingDom = writeDomNode(musicPlayerDom, "plusSetting");
    QDomElement otherSettingDom = writeDomNode(musicPlayerDom, "otherSetting");
    QDomElement rippleSettingDom = writeDomNode(musicPlayerDom, "rippleSetting");
    QDomElement backgroundSettingDom = writeDomNode(musicPlayerDom, "backgroundSetting");
    QDomElement hotkeySettingDom = writeDomNode(musicPlayerDom, "hotkeySetting");
    QDomElement interiorLrcSettingDom = writeDomNode(musicPlayerDom, "interiorLrcSetting");
    QDomElement desktopLrcSettingDom = writeDomNode(musicPlayerDom, "desktopLrcSetting");
    QDomElement equalizerSettingDom = writeDomNode(musicPlayerDom, "equalizerSetting");
    QDomElement timeSettingDom = writeDomNode(musicPlayerDom, "timeSetting");
    QDomElement downloadSettingDom = writeDomNode(musicPlayerDom, "downloadSetting");

    writeDomElement(musicSettingDom, "playMode", MusicXmlAttribute("value", playMode));
    writeDomElement(musicSettingDom, "playVolume", MusicXmlAttribute("value", volume));
    writeDomElementText(musicSettingDom, "lastPlayIndex", MusicXmlAttribute("value", lastPlayIndex[0]),
                                                          QString("%1,%2").arg(lastPlayIndex[1]).arg(lastPlayIndex[2]));

    //
    writeDomElement(plusSettingDom, "geometry", MusicXmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPosition.x())
                    .arg(widgetPosition.y()).arg(widgetSize.width()).arg(widgetSize.height())));
    writeDomElement(plusSettingDom, "language", MusicXmlAttribute("value", languageIndex));
    writeDomElement(plusSettingDom, "autoPlay", MusicXmlAttribute("value", autoPlay));
    writeDomElement(plusSettingDom, "closeEvent", MusicXmlAttribute("value", closeEvent));
    writeDomElement(plusSettingDom, "closeNetwork", MusicXmlAttribute("value", closeNetWork));
    writeDomElement(plusSettingDom, "fileAssociation", MusicXmlAttribute("value", fileAssociation));
    writeDomElement(plusSettingDom, "windowConcise", MusicXmlAttribute("value", windowConcise));
    writeDomElement(plusSettingDom, "remoteWidgetMode", MusicXmlAttribute("value", remoteWidgetMode));
    writeDomElement(plusSettingDom, "windowQuitMode", MusicXmlAttribute("value", windowQuitMode));

    //
    writeDomElement(otherSettingDom, "otherBackgroundLossless", MusicXmlAttribute("value", otherBackgroundLossless));
    writeDomElement(otherSettingDom, "otherCheckUpdate", MusicXmlAttribute("value", otherCheckUpdate));
    writeDomElement(otherSettingDom, "otherSearch", MusicXmlAttribute("value", otherSearch));
    writeDomElement(otherSettingDom, "otherUseAlbumCover", MusicXmlAttribute("value", otherUseAlbumCover));
    writeDomElement(otherSettingDom, "otherUseInfo", MusicXmlAttribute("value", otherUseInfo));
    writeDomElement(otherSettingDom, "otherWriteAlbumCover", MusicXmlAttribute("value", otherWriteAlbumCover));
    writeDomElement(otherSettingDom, "otherWriteInfo", MusicXmlAttribute("value", otherWriteInfo));
    writeDomElement(otherSettingDom, "otherSideBy", MusicXmlAttribute("value", otherSideBy));
    writeDomElement(otherSettingDom, "otherSongFormat", MusicXmlAttribute("value", otherSongFormat));
    writeDomElement(otherSettingDom, "otherLrcKTVMode", MusicXmlAttribute("value", otherLrcKTVMode));
    writeDomElement(otherSettingDom, "otherScreenSaverEnable", MusicXmlAttribute("value", otherScreenSaverEnable));
    writeDomElement(otherSettingDom, "otherScreenSaverTime", MusicXmlAttribute("value", otherScreenSaverTime));
    writeDomElement(otherSettingDom, "otherScreenSaverIndex", MusicXmlAttribute("value", otherScreenSaverIndex));

    //
    writeDomElement(rippleSettingDom, "rippleSpectrumEnable", MusicXmlAttribute("value", rippleSpectrumEnable));
    writeDomElement(rippleSettingDom, "rippleSpectrumColor", MusicXmlAttribute("value", rippleSpectrumColor));

    //
    writeDomElement(backgroundSettingDom, "backgroundTheme", MusicXmlAttribute("value", backgroundTheme));
    writeDomElement(backgroundSettingDom, "backgroundTransparent", MusicXmlAttribute("value", backgroundTransparent));
    writeDomElement(backgroundSettingDom, "backgroundListTransparent", MusicXmlAttribute("value", backgroundListTransparent));
    writeDomElement(backgroundSettingDom, "backgroundTransparentEnable", MusicXmlAttribute("value", backgroundTransparentEnable));

    //
    writeDomElement(hotkeySettingDom, "hotkeyEnable", MusicXmlAttribute("value", hotkeyEnable));
    writeDomElement(hotkeySettingDom, "hotkeyString", MusicXmlAttribute("value", hotkeyString));

    //
    writeDomElement(interiorLrcSettingDom, "showInteriorLrc", MusicXmlAttribute("value", showInteriorLrc));
    writeDomElement(interiorLrcSettingDom, "lrcColor", MusicXmlAttribute("value", lrcColor));
    writeDomElement(interiorLrcSettingDom, "lrcSize", MusicXmlAttribute("value", lrcSize));
    writeDomElement(interiorLrcSettingDom, "lrcFamily", MusicXmlAttribute("value", lrcFamily));
    writeDomElement(interiorLrcSettingDom, "lrcType", MusicXmlAttribute("value", lrcType));
    writeDomElement(interiorLrcSettingDom, "lrcTransparent", MusicXmlAttribute("value", lrcTransparent));
    writeDomElement(interiorLrcSettingDom, "lrcFrontgroundColor", MusicXmlAttribute("value", lrcFrontgroundColor));
    writeDomElement(interiorLrcSettingDom, "lrcBackgroundColor", MusicXmlAttribute("value", lrcBackgroundColor));

    //
    writeDomElement(desktopLrcSettingDom, "showDesktopLrc", MusicXmlAttribute("value", showDesktopLrc));
    writeDomElement(desktopLrcSettingDom, "lrcDColor", MusicXmlAttribute("value", DLrcColor));
    writeDomElement(desktopLrcSettingDom, "lrcDSize", MusicXmlAttribute("value", DLrcSize));
    writeDomElement(desktopLrcSettingDom, "lrcDFamily", MusicXmlAttribute("value", DLrcFamily));
    writeDomElement(desktopLrcSettingDom, "lrcDType", MusicXmlAttribute("value", DLrcType));
    writeDomElement(desktopLrcSettingDom, "lrcDTransparent", MusicXmlAttribute("value", DLrcTransparent));
    writeDomElement(desktopLrcSettingDom, "lrcDFrontgroundColor", MusicXmlAttribute("value", DLrcFrontgroundColor));
    writeDomElement(desktopLrcSettingDom, "lrcDBackgroundColor", MusicXmlAttribute("value", DLrcBackgroundColor));
    writeDomElement(desktopLrcSettingDom, "lrcDWindowType", MusicXmlAttribute("value", DLrcWindowType));
    writeDomElement(desktopLrcSettingDom, "lrcDSingleLineType", MusicXmlAttribute("value", DLrcSingleLineType));
    writeDomElement(desktopLrcSettingDom, "lrcDLocked", MusicXmlAttribute("value", DLrcLocked));
    writeDomElement(desktopLrcSettingDom, "lrcDGeometry", MusicXmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));

    //
    writeDomElement(equalizerSettingDom, "enhancedMusic", MusicXmlAttribute("value", enhancedMusic));
    writeDomElement(equalizerSettingDom, "equalizerEnable", MusicXmlAttribute("value", equalizerEnable));
    writeDomElement(equalizerSettingDom, "equalizerIndex", MusicXmlAttribute("value", equalizerIndex));
    writeDomElement(equalizerSettingDom, "equalizerValue", MusicXmlAttribute("value", equalizerValue));
    writeDomElement(equalizerSettingDom, "enhancedFadeEnable", MusicXmlAttribute("value", enhancedFadeEnable));
    writeDomElement(equalizerSettingDom, "enhancedFadeInValue", MusicXmlAttribute("value", enhancedFadeInValue));
    writeDomElement(equalizerSettingDom, "enhancedFadeOutValue", MusicXmlAttribute("value", enhancedFadeOutValue));
    writeDomElement(equalizerSettingDom, "enhancedBS2B", MusicXmlAttribute("value", enhancedBS2B));
    writeDomElement(equalizerSettingDom, "enhancedCrossfade", MusicXmlAttribute("value", enhancedCrossfade));
    writeDomElement(equalizerSettingDom, "enhancedStereo", MusicXmlAttribute("value", enhancedStereo));
    writeDomElement(equalizerSettingDom, "enhancedLADSPA", MusicXmlAttribute("value", enhancedLADSPA));
    writeDomElement(equalizerSettingDom, "enhancedSOX", MusicXmlAttribute("value", enhancedSOX));
    writeDomElement(equalizerSettingDom, "enhancedSRC", MusicXmlAttribute("value", enhancedSRC));
    writeDomElement(equalizerSettingDom, "enhancedMonoToStereo", MusicXmlAttribute("value", enhancedMonoToStereo));
    writeDomElement(equalizerSettingDom, "enhancedMono", MusicXmlAttribute("value", enhancedMono));

    //
    writeDomElement(timeSettingDom, "timeAutoIndex", MusicXmlAttribute("value", timeAutoIndex));
    writeDomElement(timeSettingDom, "timeAutoPlay", MusicXmlAttribute("value", timeAutoPlay));
    writeDomElement(timeSettingDom, "timeAutoPlayHour", MusicXmlAttribute("value", timeAutoPlayHour));
    writeDomElement(timeSettingDom, "timeAutoPlaySecond", MusicXmlAttribute("value", timeAutoPlaySecond));
    writeDomElement(timeSettingDom, "timeAutoPlayRepeat", MusicXmlAttribute("value", timeAutoPlayRepeat));
    writeDomElement(timeSettingDom, "timeAutoPlayItemIndex", MusicXmlAttribute("value", timeAutoPlayItemIndex));
    writeDomElement(timeSettingDom, "timeAutoPlaySongIndex", MusicXmlAttribute("value", timeAutoPlaySongIndex));
    writeDomElement(timeSettingDom, "timeAutoStop", MusicXmlAttribute("value", timeAutoStop));
    writeDomElement(timeSettingDom, "timeAutoStopHour", MusicXmlAttribute("value", timeAutoStopHour));
    writeDomElement(timeSettingDom, "timeAutoStopSecond", MusicXmlAttribute("value", timeAutoStopSecond));
    writeDomElement(timeSettingDom, "timeAutoStopRepeat", MusicXmlAttribute("value", timeAutoStopRepeat));
    writeDomElement(timeSettingDom, "timeAutoShutdown", MusicXmlAttribute("value", timeAutoShutdown));
    writeDomElement(timeSettingDom, "timeAutoShutdownHour", MusicXmlAttribute("value", timeAutoShutdownHour));
    writeDomElement(timeSettingDom, "timeAutoShutdownSecond", MusicXmlAttribute("value", timeAutoShutdownSecond));
    writeDomElement(timeSettingDom, "timeAutoShutdownRepeat", MusicXmlAttribute("value", timeAutoShutdownRepeat));

    //
    writeDomElement(downloadSettingDom, "downloadMusicPath", MusicXmlAttribute("value", downloadMusicPath));
    writeDomElement(downloadSettingDom, "downloadLrcPath", MusicXmlAttribute("value", downloadLrcPath));
    writeDomElement(downloadSettingDom, "downloadCacheLimit", MusicXmlAttribute("value", downloadCacheLimit));
    writeDomElement(downloadSettingDom, "downloadCacheSize", MusicXmlAttribute("value", downloadCacheSize));
    writeDomElement(downloadSettingDom, "downloadLimit", MusicXmlAttribute("value", downloadLimit));
    writeDomElement(downloadSettingDom, "downloadServer", MusicXmlAttribute("value", downloadServer));
    writeDomElement(downloadSettingDom, "downloadDLoadLimit", MusicXmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloadSettingDom, "downloadULoadLimit", MusicXmlAttribute("value", downloadULoadLimit));

    QTextStream out(m_file);
    m_document->save(out, 4);
}

void MusicConfigManager::readSystemLastPlayIndexConfig(QStringList &key) const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("lastPlayIndex");
    if(nodeList.isEmpty())
    {
        key << "0" << "0" << "-1";
        return;
    }

    const QDomElement &element = nodeList.at(0).toElement();
    key << element.attribute("value") << element.text().split(",");
    if(key.count() != 3)
    {
        key.clear();
        key << "0" << "0" << "-1";
    }
}

QRect MusicConfigManager::readWindowGeometry() const
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("geometry");
    if(nodeList.isEmpty())
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }

    const QDomElement &element = nodeList.at(0).toElement();
    const QStringList &lists = element.attribute("value").split(",");
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

QPoint MusicConfigManager::readShowDLrcGeometry() const
{
    const QStringList &point = readXmlAttributeByTagNameValue("lrcDGeometry").split(",");
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}
