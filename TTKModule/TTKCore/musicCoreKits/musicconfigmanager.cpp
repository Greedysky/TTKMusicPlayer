#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"
#include "ttkversion.h"

#include <QRect>

MusicConfigManager::MusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicConfigManager::readSysConfigData() const
{
    G_SETTING_PTR->setValue(MusicSettingManager::ConfigVersion,
                     readXmlAttributeByTagNameValue("configVersion"));


    G_SETTING_PTR->setValue(MusicSettingManager::PlayMode,
                     readXmlAttributeByTagNameValue("playMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Volume,
                     readXmlAttributeByTagNameValue("playVolume").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndex,
                     readXmlAttributeByTagNameValue("language").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::AutoPlayMode,
                     readXmlAttributeByTagNameValue("autoPlayMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseEventMode,
                     readXmlAttributeByTagNameValue("closeEventMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseNetWorkMode,
                     readXmlAttributeByTagNameValue("closeNetworkMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode,
                     readXmlAttributeByTagNameValue("fileAssociationMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode,
                     readXmlAttributeByTagNameValue("windowConciseMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode,
                     readXmlAttributeByTagNameValue("remoteWidgetMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode,
                     readXmlAttributeByTagNameValue("windowQuitMode").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateEnable,
                     readXmlAttributeByTagNameValue("otherCheckUpdateEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSearchOptimized,
                     readXmlAttributeByTagNameValue("otherSearchOptimized").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCover,
                     readXmlAttributeByTagNameValue("otherUseAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherUseFileInfo,
                     readXmlAttributeByTagNameValue("otherUseFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover,
                     readXmlAttributeByTagNameValue("otherWriteAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteFileInfo,
                     readXmlAttributeByTagNameValue("otherWriteFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByMode,
                     readXmlAttributeByTagNameValue("otherSideByMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode,
                     readXmlAttributeByTagNameValue("otherLrcKTVMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable,
                     readXmlAttributeByTagNameValue("otherScreenSaverEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime,
                     readXmlAttributeByTagNameValue("otherScreenSaverTime").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex,
                     readXmlAttributeByTagNameValue("otherScreenSaverIndex"));


    G_SETTING_PTR->setValue(MusicSettingManager::RippleLowPowerMode,
                     readXmlAttributeByTagNameValue("rippleLowPowerMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable,
                     readXmlAttributeByTagNameValue("rippleSpectrumEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor,
                     readXmlAttributeByTagNameValue("rippleSpectrumColor"));


    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeValue,
                     readXmlAttributeByTagNameValue("backgroundThemeValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent,
                     readXmlAttributeByTagNameValue("backgroundTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable,
                     readXmlAttributeByTagNameValue("backgroundTransparentEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent,
                     readXmlAttributeByTagNameValue("backgroundListTransparent").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable,
                     readXmlAttributeByTagNameValue("hotkeyEnable"));
    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyValue,
                     readXmlAttributeByTagNameValue("hotkeyValue"));


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
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransparent,
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
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransparent,
                     readXmlAttributeByTagNameValue("lrcDTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowMode,
                     readXmlAttributeByTagNameValue("lrcDWindowMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineMode,
                     readXmlAttributeByTagNameValue("lrcDSingleLineMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcLockedMode,
                     readXmlAttributeByTagNameValue("lrcDLockedMode").toInt());


    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicIndex,
                     readXmlAttributeByTagNameValue("enhancedMusicIndex").toInt());
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
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedEffectValue,
                     readXmlAttributeByTagNameValue("enhancedEffectValue"));


    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndex,
                     readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayMode,
                     readXmlAttributeByTagNameValue("timeAutoPlayMode").toInt());
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
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopMode,
                     readXmlAttributeByTagNameValue("timeAutoStopMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHour,
                     readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecond,
                     readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeat,
                     readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownMode,
                     readXmlAttributeByTagNameValue("timeAutoShutdownMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHour,
                     readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecond,
                     readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeat,
                     readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());


    QString path = readXmlAttributeByTagNameValue("downloadMusicPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::musicPrefix() : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcDirPath,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::lrcPrefix() : path);


    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheEnable,
                     readXmlAttributeByTagNameValue("downloadCacheEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize,
                     readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable,
                     readXmlAttributeByTagNameValue("downloadLimitEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex,
                     readXmlAttributeByTagNameValue("downloadServerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadDownloadLimitSize,
                     readXmlAttributeByTagNameValue("downloadDownloadLimitSize"));
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadUploadLimitSize,
                     readXmlAttributeByTagNameValue("downloadUploadLimitSize"));
}

void MusicConfigManager::writeSysConfigData()
{
    const int playMode = G_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt();
    const int volume = G_SETTING_PTR->value(MusicSettingManager::Volume).toInt();
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    //
    const QPoint &widgetPosition = G_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    const QSize &widgetSize = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    const int autoPlayMode = G_SETTING_PTR->value(MusicSettingManager::AutoPlayMode).toInt();
    const int languageIndex = G_SETTING_PTR->value(MusicSettingManager::CurrentLanIndex).toInt();
    const int closeEventMode = G_SETTING_PTR->value(MusicSettingManager::CloseEventMode).toInt();
    const int closeNetWorkMode = G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toInt();
    const int fileAssociationMode = G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toInt();
    const int windowConciseMode = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toInt();
    const int remoteWidgetMode = G_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    const int windowQuitMode = G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toInt();
    //
    const int otherCheckUpdateEnable = G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toInt();
    const int otherSearchOptimized = G_SETTING_PTR->value(MusicSettingManager::OtherSearchOptimized).toInt();
    const int otherUseAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCover).toInt();
    const int otherUseFileInfo = G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toInt();
    const int otherWriteAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toInt();
    const int otherWriteFileInfo = G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toInt();
    const int otherSideByMode = G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toInt();
    const int otherLrcKTVMode = G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toInt();
    const int otherScreenSaverEnable = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toInt();
    const int otherScreenSaverTime = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    const QString &otherScreenSaverIndex = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();
    //
    const int rippleLowPowerMode = G_SETTING_PTR->value(MusicSettingManager::RippleLowPowerMode).toInt();
    const int rippleSpectrumEnable = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumEnable).toInt();
    const QString &rippleSpectrumColor = G_SETTING_PTR->value(MusicSettingManager::RippleSpectrumColor).toString();
    //
    const QString &backgroundThemeValue = G_SETTING_PTR->value(MusicSettingManager::BackgroundThemeValue).toString();
    const int backgroundTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparent).toInt();
    const int backgroundListTransparent = G_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparent).toInt();
    const int backgroundTransparentEnable = G_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnable).toInt();
    //
    const int hotkeyEnable = G_SETTING_PTR->value(MusicSettingManager::HotkeyEnable).toInt();
    const QString &hotkeyValue = G_SETTING_PTR->value(MusicSettingManager::HotkeyValue).toString();
    //
    const int showInteriorLrc = G_SETTING_PTR->value(MusicSettingManager::ShowInteriorLrc).toInt();
    const int showDesktopLrc = G_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrc).toInt();
    const int lrcColor = G_SETTING_PTR->value(MusicSettingManager::LrcColor).toInt();
    const int lrcSize = G_SETTING_PTR->value(MusicSettingManager::LrcSize).toInt();
    const int lrcType = G_SETTING_PTR->value(MusicSettingManager::LrcType).toInt();
    const int lrcFamily = G_SETTING_PTR->value(MusicSettingManager::LrcFamily).toInt();
    const QString &lrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColor).toString();
    const QString &lrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColor).toString();
    const int lrcTransparent = G_SETTING_PTR->value(MusicSettingManager::LrcColorTransparent).toInt();
    //
    const int DLrcColor = G_SETTING_PTR->value(MusicSettingManager::DLrcColor).toInt();
    const int DLrcSize = G_SETTING_PTR->value(MusicSettingManager::DLrcSize).toInt();
    const int DLrcType = G_SETTING_PTR->value(MusicSettingManager::DLrcType).toInt();
    const int DLrcFamily = G_SETTING_PTR->value(MusicSettingManager::DLrcFamily).toInt();
    const QString &DLrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColor).toString();
    const QString &DLrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColor).toString();
    const int DLrcTransparent = G_SETTING_PTR->value(MusicSettingManager::DLrcColorTransparent).toInt();
    const int DLrcWindowMode= G_SETTING_PTR->value(MusicSettingManager::DLrcWindowMode).toInt();
    const int DLrcSingleLineMode = G_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineMode).toInt();
    const int DLrcLockedMode = G_SETTING_PTR->value(MusicSettingManager::DLrcLockedMode).toInt();
    const QPoint &DLrcGeometry = G_SETTING_PTR->value(MusicSettingManager::DLrcGeometry).toPoint();
    //
    const int enhancedMusicIndex = G_SETTING_PTR->value(MusicSettingManager::EnhancedMusicIndex).toInt();
    const int equalizerEnable = G_SETTING_PTR->value(MusicSettingManager::EqualizerEnable).toInt();
    const QString &equalizerValue = G_SETTING_PTR->value(MusicSettingManager::EqualizerValue).toString();
    const int equalizerIndex = G_SETTING_PTR->value(MusicSettingManager::EqualizerIndex).toInt();
    const int enhancedFadeEnable = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnable).toInt();
    const int enhancedFadeInValue = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValue).toInt();
    const int enhancedFadeOutValue = G_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValue).toInt();
    const QString &enhancedEffectValue = G_SETTING_PTR->value(MusicSettingManager::EnhancedEffectValue).toString();
    //
    const int timeAutoIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoIndex).toInt();
    const int timeAutoPlayMode = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayMode).toInt();
    const int timeAutoPlayHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHour).toInt();
    const int timeAutoPlaySecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecond).toInt();
    const int timeAutoPlayRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeat).toInt();
    const int timeAutoPlayItemIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndex).toInt();
    const int timeAutoPlaySongIndex = G_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndex).toInt();
    const int timeAutoStopMode = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopMode).toInt();
    const int timeAutoStopHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHour).toInt();
    const int timeAutoStopSecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecond).toInt();
    const int timeAutoStopRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeat).toInt();
    const int timeAutoShutdownMode = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownMode).toInt();
    const int timeAutoShutdownHour = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHour).toInt();
    const int timeAutoShutdownSecond = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecond).toInt();
    const int timeAutoShutdownRepeat = G_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeat).toInt();
    //
    const QString &downloadMusicPath = G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString();
    const QString &downloadLrcPath = G_SETTING_PTR->value(MusicSettingManager::DownloadLrcDirPath).toString();
    const int downloadCacheEnable = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheEnable).toInt();
    const int downloadCacheSize = G_SETTING_PTR->value(MusicSettingManager::DownloadCacheSize).toInt();
    const int downloadLimitEnable = G_SETTING_PTR->value(MusicSettingManager::DownloadLimitEnable).toInt();
    const int downloadServerIndex = G_SETTING_PTR->value(MusicSettingManager::DownloadServerIndex).toInt();
    const QString &downloadDownloadLimitSize = G_SETTING_PTR->value(MusicSettingManager::DownloadDownloadLimitSize).toString();
    const QString &downloadUploadLimitSize = G_SETTING_PTR->value(MusicSettingManager::DownloadUploadLimitSize).toString();
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

    writeDomElement(musicSettingDom, "configVersion", MusicXmlAttribute("value", TTKCONFIG_VERSION_STR));
    writeDomElement(musicSettingDom, "playMode", MusicXmlAttribute("value", playMode));
    writeDomElement(musicSettingDom, "playVolume", MusicXmlAttribute("value", volume));
    writeDomElementText(musicSettingDom, "lastPlayIndex", MusicXmlAttribute("value", lastPlayIndex[0]), QString("%1,%2").arg(lastPlayIndex[1]).arg(lastPlayIndex[2]));
    //
    writeDomElement(plusSettingDom, "geometry", MusicXmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPosition.x()).arg(widgetPosition.y()).arg(widgetSize.width()).arg(widgetSize.height())));
    writeDomElement(plusSettingDom, "language", MusicXmlAttribute("value", languageIndex));
    writeDomElement(plusSettingDom, "autoPlayMode", MusicXmlAttribute("value", autoPlayMode));
    writeDomElement(plusSettingDom, "closeEventMode", MusicXmlAttribute("value", closeEventMode));
    writeDomElement(plusSettingDom, "closeNetworkMode", MusicXmlAttribute("value", closeNetWorkMode));
    writeDomElement(plusSettingDom, "fileAssociationMode", MusicXmlAttribute("value", fileAssociationMode));
    writeDomElement(plusSettingDom, "windowConciseMode", MusicXmlAttribute("value", windowConciseMode));
    writeDomElement(plusSettingDom, "remoteWidgetMode", MusicXmlAttribute("value", remoteWidgetMode));
    writeDomElement(plusSettingDom, "windowQuitMode", MusicXmlAttribute("value", windowQuitMode));

    //
    writeDomElement(otherSettingDom, "otherCheckUpdateEnable", MusicXmlAttribute("value", otherCheckUpdateEnable));
    writeDomElement(otherSettingDom, "otherSearchOptimized", MusicXmlAttribute("value", otherSearchOptimized));
    writeDomElement(otherSettingDom, "otherUseAlbumCover", MusicXmlAttribute("value", otherUseAlbumCover));
    writeDomElement(otherSettingDom, "otherUseFileInfo", MusicXmlAttribute("value", otherUseFileInfo));
    writeDomElement(otherSettingDom, "otherWriteAlbumCover", MusicXmlAttribute("value", otherWriteAlbumCover));
    writeDomElement(otherSettingDom, "otherWriteFileInfo", MusicXmlAttribute("value", otherWriteFileInfo));
    writeDomElement(otherSettingDom, "otherSideByMode", MusicXmlAttribute("value", otherSideByMode));
    writeDomElement(otherSettingDom, "otherLrcKTVMode", MusicXmlAttribute("value", otherLrcKTVMode));
    writeDomElement(otherSettingDom, "otherScreenSaverEnable", MusicXmlAttribute("value", otherScreenSaverEnable));
    writeDomElement(otherSettingDom, "otherScreenSaverTime", MusicXmlAttribute("value", otherScreenSaverTime));
    writeDomElement(otherSettingDom, "otherScreenSaverIndex", MusicXmlAttribute("value", otherScreenSaverIndex));
    //
    writeDomElement(rippleSettingDom, "rippleLowPowerMode", MusicXmlAttribute("value", rippleLowPowerMode));
    writeDomElement(rippleSettingDom, "rippleSpectrumEnable", MusicXmlAttribute("value", rippleSpectrumEnable));
    writeDomElement(rippleSettingDom, "rippleSpectrumColor", MusicXmlAttribute("value", rippleSpectrumColor));
    //
    writeDomElement(backgroundSettingDom, "backgroundThemeValue", MusicXmlAttribute("value", backgroundThemeValue));
    writeDomElement(backgroundSettingDom, "backgroundTransparent", MusicXmlAttribute("value", backgroundTransparent));
    writeDomElement(backgroundSettingDom, "backgroundListTransparent", MusicXmlAttribute("value", backgroundListTransparent));
    writeDomElement(backgroundSettingDom, "backgroundTransparentEnable", MusicXmlAttribute("value", backgroundTransparentEnable));
    //
    writeDomElement(hotkeySettingDom, "hotkeyEnable", MusicXmlAttribute("value", hotkeyEnable));
    writeDomElement(hotkeySettingDom, "hotkeyValue", MusicXmlAttribute("value", hotkeyValue));
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
    writeDomElement(desktopLrcSettingDom, "lrcDWindowMode", MusicXmlAttribute("value", DLrcWindowMode));
    writeDomElement(desktopLrcSettingDom, "lrcDSingleLineMode", MusicXmlAttribute("value", DLrcSingleLineMode));
    writeDomElement(desktopLrcSettingDom, "lrcDLockedMode", MusicXmlAttribute("value", DLrcLockedMode));
    writeDomElement(desktopLrcSettingDom, "lrcDGeometry", MusicXmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));
    //
    writeDomElement(equalizerSettingDom, "enhancedMusicIndex", MusicXmlAttribute("value", enhancedMusicIndex));
    writeDomElement(equalizerSettingDom, "equalizerEnable", MusicXmlAttribute("value", equalizerEnable));
    writeDomElement(equalizerSettingDom, "equalizerIndex", MusicXmlAttribute("value", equalizerIndex));
    writeDomElement(equalizerSettingDom, "equalizerValue", MusicXmlAttribute("value", equalizerValue));
    writeDomElement(equalizerSettingDom, "enhancedFadeEnable", MusicXmlAttribute("value", enhancedFadeEnable));
    writeDomElement(equalizerSettingDom, "enhancedFadeInValue", MusicXmlAttribute("value", enhancedFadeInValue));
    writeDomElement(equalizerSettingDom, "enhancedFadeOutValue", MusicXmlAttribute("value", enhancedFadeOutValue));
    writeDomElement(equalizerSettingDom, "enhancedEffectValue", MusicXmlAttribute("value", enhancedEffectValue));
    //
    writeDomElement(timeSettingDom, "timeAutoIndex", MusicXmlAttribute("value", timeAutoIndex));
    writeDomElement(timeSettingDom, "timeAutoPlayMode", MusicXmlAttribute("value", timeAutoPlayMode));
    writeDomElement(timeSettingDom, "timeAutoPlayHour", MusicXmlAttribute("value", timeAutoPlayHour));
    writeDomElement(timeSettingDom, "timeAutoPlaySecond", MusicXmlAttribute("value", timeAutoPlaySecond));
    writeDomElement(timeSettingDom, "timeAutoPlayRepeat", MusicXmlAttribute("value", timeAutoPlayRepeat));
    writeDomElement(timeSettingDom, "timeAutoPlayItemIndex", MusicXmlAttribute("value", timeAutoPlayItemIndex));
    writeDomElement(timeSettingDom, "timeAutoPlaySongIndex", MusicXmlAttribute("value", timeAutoPlaySongIndex));
    writeDomElement(timeSettingDom, "timeAutoStopMode", MusicXmlAttribute("value", timeAutoStopMode));
    writeDomElement(timeSettingDom, "timeAutoStopHour", MusicXmlAttribute("value", timeAutoStopHour));
    writeDomElement(timeSettingDom, "timeAutoStopSecond", MusicXmlAttribute("value", timeAutoStopSecond));
    writeDomElement(timeSettingDom, "timeAutoStopRepeat", MusicXmlAttribute("value", timeAutoStopRepeat));
    writeDomElement(timeSettingDom, "timeAutoShutdownMode", MusicXmlAttribute("value", timeAutoShutdownMode));
    writeDomElement(timeSettingDom, "timeAutoShutdownHour", MusicXmlAttribute("value", timeAutoShutdownHour));
    writeDomElement(timeSettingDom, "timeAutoShutdownSecond", MusicXmlAttribute("value", timeAutoShutdownSecond));
    writeDomElement(timeSettingDom, "timeAutoShutdownRepeat", MusicXmlAttribute("value", timeAutoShutdownRepeat));
    //
    writeDomElement(downloadSettingDom, "downloadMusicPath", MusicXmlAttribute("value", downloadMusicPath));
    writeDomElement(downloadSettingDom, "downloadLrcPath", MusicXmlAttribute("value", downloadLrcPath));
    writeDomElement(downloadSettingDom, "downloadCacheEnable", MusicXmlAttribute("value", downloadCacheEnable));
    writeDomElement(downloadSettingDom, "downloadCacheSize", MusicXmlAttribute("value", downloadCacheSize));
    writeDomElement(downloadSettingDom, "downloadLimitEnable", MusicXmlAttribute("value", downloadLimitEnable));
    writeDomElement(downloadSettingDom, "downloadServerIndex", MusicXmlAttribute("value", downloadServerIndex));
    writeDomElement(downloadSettingDom, "downloadDownloadLimitSize", MusicXmlAttribute("value", downloadDownloadLimitSize));
    writeDomElement(downloadSettingDom, "downloadUploadLimitSize", MusicXmlAttribute("value", downloadUploadLimitSize));

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
    const QStringList &list = element.attribute("value").split(",");
    if(list.count() == 4)
    {
        return QRect(list[0].toInt() < 0 ? 0 : list[0].toInt(),
                     list[1].toInt() < 0 ? 0 : list[1].toInt(),
                     list[2].toInt(), list[3].toInt());
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
