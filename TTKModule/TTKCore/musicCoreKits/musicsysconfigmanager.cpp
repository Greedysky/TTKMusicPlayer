#include "musicsysconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"

#include <QRect>

MusicSysConfigManager::MusicSysConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSysConfigManager::readSysConfigData() const
{
    M_SETTING_PTR->setValue(MusicSettingManager::PlayMode,
                     readXmlAttributeByTagNameValue("playMode").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::Volume,
                     readXmlAttributeByTagNameValue("playVolume").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndex,
                     readXmlAttributeByTagNameValue("language").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::AutoPlay,
                     readXmlAttributeByTagNameValue("autoPlay").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::CloseEvent,
                     readXmlAttributeByTagNameValue("closeEvent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::CloseNetWork,
                     readXmlAttributeByTagNameValue("closeNetwork").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociation,
                     readXmlAttributeByTagNameValue("fileAssociation").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConcise,
                     readXmlAttributeByTagNameValue("windowConcise").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode,
                     readXmlAttributeByTagNameValue("remoteWidgetMode").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode,
                     readXmlAttributeByTagNameValue("windowQuitMode").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::OtherBackgroundLossless,
                     readXmlAttributeByTagNameValue("otherBackgroundLossless").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdate,
                     readXmlAttributeByTagNameValue("otherCheckUpdate").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSearch,
                     readXmlAttributeByTagNameValue("otherSearch").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCover,
                     readXmlAttributeByTagNameValue("otherUseAlbumCover").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseInfo,
                     readXmlAttributeByTagNameValue("otherUseInfo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover,
                     readXmlAttributeByTagNameValue("otherWriteAlbumCover").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteInfo,
                     readXmlAttributeByTagNameValue("otherWriteInfo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSideBy,
                     readXmlAttributeByTagNameValue("otherSideBy").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSongFormat,
                     readXmlAttributeByTagNameValue("otherSongFormat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode,
                     readXmlAttributeByTagNameValue("otherLrcKTVMode").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable,
                     readXmlAttributeByTagNameValue("otherScreenSaverEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime,
                     readXmlAttributeByTagNameValue("otherScreenSaverTime").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex,
                     readXmlAttributeByTagNameValue("otherScreenSaverIndex"));


    M_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable,
                     readXmlAttributeByTagNameValue("rippleSpectrumEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor,
                     readXmlAttributeByTagNameValue("rippleSpectrumColor"));


    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTheme,
                     readXmlAttributeByTagNameValue("backgroundTheme"));
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent,
                     readXmlAttributeByTagNameValue("backgroundTransparent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable,
                     readXmlAttributeByTagNameValue("backgroundTransparentEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent,
                     readXmlAttributeByTagNameValue("backgroundListTransparent").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable,
                     readXmlAttributeByTagNameValue("hotkeyEnable"));
    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyString,
                     readXmlAttributeByTagNameValue("hotkeyString"));


    M_SETTING_PTR->setValue(MusicSettingManager::ShowInteriorLrc,
                     readXmlAttributeByTagNameValue("showInteriorLrc").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColor,
                     readXmlAttributeByTagNameValue("lrcColor").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSize,
                     readXmlAttributeByTagNameValue("lrcSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFamily,
                     readXmlAttributeByTagNameValue("lrcFamily"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColor,
                     readXmlAttributeByTagNameValue("lrcFrontgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColor,
                     readXmlAttributeByTagNameValue("lrcBackgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcType,
                     readXmlAttributeByTagNameValue("lrcType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorTrans,
                     readXmlAttributeByTagNameValue("lrcTransparent").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc,
                     readXmlAttributeByTagNameValue("showDesktopLrc").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColor,
                     readXmlAttributeByTagNameValue("lrcDColor").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSize,
                     readXmlAttributeByTagNameValue("lrcDSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFamily,
                     readXmlAttributeByTagNameValue("lrcDFamily"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColor,
                     readXmlAttributeByTagNameValue("lrcDFrontgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColor,
                     readXmlAttributeByTagNameValue("lrcDBackgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcType,
                     readXmlAttributeByTagNameValue("lrcDType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTrans,
                     readXmlAttributeByTagNameValue("lrcDTransparent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowType,
                     readXmlAttributeByTagNameValue("lrcDWindowType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineType,
                     readXmlAttributeByTagNameValue("lrcDSingleLineType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcLocked,
                     readXmlAttributeByTagNameValue("lrcDLocked").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusic,
                     readXmlAttributeByTagNameValue("enhancedMusic").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndex,
                     readXmlAttributeByTagNameValue("equalizerIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable,
                     readXmlAttributeByTagNameValue("equalizerEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValue,
                     readXmlAttributeByTagNameValue("equalizerValue"));
//    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBalance,
//                     readXmlAttributeByTagNameValue("enhancedBalance").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBalance, 0);
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable,
                     readXmlAttributeByTagNameValue("enhancedFadeEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValue,
                     readXmlAttributeByTagNameValue("enhancedFadeInValue").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValue,
                     readXmlAttributeByTagNameValue("enhancedFadeOutValue").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBS2B,
                     readXmlAttributeByTagNameValue("enhancedBS2B").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedCrossfade,
                     readXmlAttributeByTagNameValue("enhancedCrossfade").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedStereo,
                     readXmlAttributeByTagNameValue("enhancedStereo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedLADSPA,
                     readXmlAttributeByTagNameValue("enhancedLADSPA").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSOX,
                     readXmlAttributeByTagNameValue("enhancedSOX").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSRC,
                     readXmlAttributeByTagNameValue("enhancedSRC").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMonoStereo,
                     readXmlAttributeByTagNameValue("enhancedMonoStereo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMono,
                     readXmlAttributeByTagNameValue("enhancedMono").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndex,
                     readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlay,
                     readXmlAttributeByTagNameValue("timeAutoPlay").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayHour,
                     readXmlAttributeByTagNameValue("timeAutoPlayHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySecond,
                     readXmlAttributeByTagNameValue("timeAutoPlaySecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayRepeat,
                     readXmlAttributeByTagNameValue("timeAutoPlayRepeat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayItemIndex,
                     readXmlAttributeByTagNameValue("timeAutoPlayItemIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySongIndex,
                     readXmlAttributeByTagNameValue("timeAutoPlaySongIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStop,
                     readXmlAttributeByTagNameValue("timeAutoStop").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHour,
                     readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecond,
                     readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeat,
                     readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdown,
                     readXmlAttributeByTagNameValue("timeAutoShutdown").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHour,
                     readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecond,
                     readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeat,
                     readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());


    QString path = readXmlAttributeByTagNameValue("downloadMusicPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicPathDir,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::musicPrefix() : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDir,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::lrcPrefix() : path);
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheLimit,
                     readXmlAttributeByTagNameValue("downloadCacheLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLimit,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadServer,
                     readXmlAttributeByTagNameValue("downloadServer").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadDLoadLimit,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit"));
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadULoadLimit,
                     readXmlAttributeByTagNameValue("downloadULoadLimit"));
}

void MusicSysConfigManager::writeSysConfigData()
{
    const int playMode = M_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt();
    const int volume = M_SETTING_PTR->value(MusicSettingManager::Volume).toInt();
    const QStringList &lastPlayIndex = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();

    //
    const QPoint &widgetPosition = M_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    const QSize &widgetSize = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    const int autoPlay = M_SETTING_PTR->value(MusicSettingManager::AutoPlay).toInt();
    const int languageIndex = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    const int closeEvent = M_SETTING_PTR->value(MusicSettingManager::CloseEvent).toInt();
    const int closeNetWork = M_SETTING_PTR->value(MusicSettingManager::CloseNetWork).toInt();
    const int fileAssociation = M_SETTING_PTR->value(MusicSettingManager::FileAssociation).toInt();
    const int windowConcise = M_SETTING_PTR->value(MusicSettingManager::WindowConcise).toInt();
    const int remoteWidgetMode = M_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    const int windowQuitMode = M_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toInt();

    //
    const int otherBackgroundLossless = M_SETTING_PTR->value(MusicSettingManager::OtherBackgroundLossless).toInt();
    const int otherCheckUpdate = M_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdate).toInt();
    const int otherSearch = M_SETTING_PTR->value(MusicSettingManager::OtherSearch).toInt();
    const int otherUseAlbumCover = M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toInt();
    const int otherUseInfo = M_SETTING_PTR->value(MusicSettingManager::OtherUseInfo).toInt();
    const int otherWriteAlbumCover = M_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toInt();
    const int otherWriteInfo = M_SETTING_PTR->value(MusicSettingManager::OtherWriteInfo).toInt();
    const int otherSideBy = M_SETTING_PTR->value(MusicSettingManager::OtherSideBy).toInt();
    const int otherSongFormat = M_SETTING_PTR->value(MusicSettingManager::OtherSongFormat).toInt();
    const int otherLrcKTVMode = M_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toInt();
    const int otherScreenSaverEnable = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toInt();
    const int otherScreenSaverTime = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    const QString &otherScreenSaverIndex = M_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();

    //
    const int rippleSpectrumEnable = M_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toInt();
    const QString &rippleSpectrumColor = M_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();

    //
    const QString &backgroundTheme = M_SETTING_PTR->value(MusicSettingManager::BackgroundTheme).toString();
    const int backgroundTransparent = M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
    const int backgroundListTransparent = M_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
    const int backgroundTransparentEnable = M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnable).toInt();

    //
    const int hotkeyEnable = M_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toInt();
    const QString &hotkeyString = M_SETTING_PTR->value(MusicSettingManager::HotkeyString).toString();

    //
    const int showInteriorLrc = M_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toInt();
    const int showDesktopLrc = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt();
    const int lrcColor = M_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt();
    const int lrcSize = M_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
    const int lrcType = M_SETTING_PTR->value(MusicSettingManager::LrcType).toInt();
    const int lrcFamily = M_SETTING_PTR->value(MusicSettingManager::LrcFamily).toInt();
    const QString &lrcFrontgroundColor = M_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColor).toString();
    const QString &lrcBackgroundColor = M_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColor).toString();
    const int lrcTransparent = M_SETTING_PTR->value(MusicSettingManager::LrcColorTrans).toInt();

    //
    const int DLrcColor = M_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt();
    const int DLrcSize = M_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt();
    const int DLrcType = M_SETTING_PTR->value(MusicSettingManager::DLrcType).toInt();
    const int DLrcFamily = M_SETTING_PTR->value(MusicSettingManager::DLrcFamily).toInt();
    const QString &DLrcFrontgroundColor = M_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColor).toString();
    const QString &DLrcBackgroundColor = M_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColor).toString();
    const int DLrcTransparent = M_SETTING_PTR->value(MusicSettingManager::DLrcColorTrans).toInt();
    const int DLrcWindowType = M_SETTING_PTR->value(MusicSettingManager::DLrcWindowType).toInt();
    const int DLrcSingleLineType = M_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineType).toInt();
    const int DLrcLocked = M_SETTING_PTR->value(MusicSettingManager::DLrcLocked).toInt();
    const QPoint &DLrcGeometry = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometry).toPoint();

    //
    const int enhancedMusic = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusic).toInt();
    const int equalizerEnable = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt();
    const QString &equalizerValue = M_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString();
    const int equalizerIndex = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt();
    const int enhancedBalance = M_SETTING_PTR->value(MusicSettingManager::EnhancedBalance).toInt();
    const int enhancedFadeEnable = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnable).toInt();
    const int enhancedFadeInValue = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValue).toInt();
    const int enhancedFadeOutValue = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValue).toInt();
    const int enhancedBS2B = M_SETTING_PTR->value(MusicSettingManager::EnhancedBS2B).toInt();
    const int enhancedCrossfade = M_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfade).toInt();
    const int enhancedStereo = M_SETTING_PTR->value(MusicSettingManager::EnhancedStereo).toInt();
    const int enhancedLADSPA = M_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPA).toInt();
    const int enhancedSOX = M_SETTING_PTR->value(MusicSettingManager::EnhancedSOX).toInt();
    const int enhancedSRC = M_SETTING_PTR->value(MusicSettingManager::EnhancedSRC).toInt();
    const int enhancedMonoStereo = M_SETTING_PTR->value(MusicSettingManager::EnhancedMonoStereo).toInt();
    const int enhancedMono = M_SETTING_PTR->value(MusicSettingManager::EnhancedMono).toInt();

    //
    const int timeAutoIndex = M_SETTING_PTR->value(MusicSettingManager::TimerAutoIndex).toInt();
    const int timeAutoPlay = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlay).toInt();
    const int timeAutoPlayHour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHour).toInt();
    const int timeAutoPlaySecond = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecond).toInt();
    const int timeAutoPlayRepeat = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeat).toInt();
    const int timeAutoPlayItemIndex = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndex).toInt();
    const int timeAutoPlaySongIndex = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndex).toInt();
    const int timeAutoStop = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStop).toInt();
    const int timeAutoStopHour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHour).toInt();
    const int timeAutoStopSecond = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecond).toInt();
    const int timeAutoStopRepeat = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeat).toInt();
    const int timeAutoShutdown = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdown).toInt();
    const int timeAutoShutdownHour = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHour).toInt();
    const int timeAutoShutdownSecond = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecond).toInt();
    const int timeAutoShutdownRepeat = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeat).toInt();
    //

    const QString &downloadMusicPath = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString();
    const QString &downloadLrcPath = M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDir).toString();
    const int downloadCacheLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimit).toInt();
    const int downloadCacheSize = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt();
    const int downloadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadLimit).toInt();
    const int downloadServer = M_SETTING_PTR->value(MusicSettingManager::DownloadServer).toInt();
    const QString &downloadDLoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimit).toString();
    const QString &downloadULoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimit).toString();

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
    writeDomElement(otherSettingDom, "otherUpdate", MusicXmlAttribute("value", otherCheckUpdate));
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
    writeDomElement(equalizerSettingDom, "enhancedBalance", MusicXmlAttribute("value", enhancedBalance));
    writeDomElement(equalizerSettingDom, "enhancedFadeEnable", MusicXmlAttribute("value", enhancedFadeEnable));
    writeDomElement(equalizerSettingDom, "enhancedFadeInValue", MusicXmlAttribute("value", enhancedFadeInValue));
    writeDomElement(equalizerSettingDom, "enhancedFadeOutValue", MusicXmlAttribute("value", enhancedFadeOutValue));
    writeDomElement(equalizerSettingDom, "enhancedBS2B", MusicXmlAttribute("value", enhancedBS2B));
    writeDomElement(equalizerSettingDom, "enhancedCrossfade", MusicXmlAttribute("value", enhancedCrossfade));
    writeDomElement(equalizerSettingDom, "enhancedStereo", MusicXmlAttribute("value", enhancedStereo));
    writeDomElement(equalizerSettingDom, "enhancedLADSPA", MusicXmlAttribute("value", enhancedLADSPA));
    writeDomElement(equalizerSettingDom, "enhancedSOX", MusicXmlAttribute("value", enhancedSOX));
    writeDomElement(equalizerSettingDom, "enhancedSRC", MusicXmlAttribute("value", enhancedSRC));
    writeDomElement(equalizerSettingDom, "enhancedMonoStereo", MusicXmlAttribute("value", enhancedMonoStereo));
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

void MusicSysConfigManager::readSystemLastPlayIndexConfig(QStringList &key) const
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

QRect MusicSysConfigManager::readWindowGeometry() const
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

QPoint MusicSysConfigManager::readShowDLrcGeometry() const
{
    const QStringList &point = readXmlAttributeByTagNameValue("lrcDGeometry").split(",");
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}
