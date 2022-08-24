#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"
#include "ttkversion.h"

#include <QRect>

MusicConfigManager::MusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicConfigManager::readBuffer() const
{
    G_SETTING_PTR->setValue(MusicSettingManager::Config::ConfigVersion, readXmlAttributeByTagNameValue("configVersion"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::PlayMode, readXmlAttributeByTagNameValue("playMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::Volume, readXmlAttributeByTagNameValue("playVolume").toInt());

    QString value = readXmlAttributeByTagNameValue("lastPlayIndex");
    {
        QStringList lastPlayIndex;
        if(value.isEmpty())
        {
            lastPlayIndex << "1" << "-1" << "-1";
        }
        else
        {
            lastPlayIndex << value.split(",");
            if(lastPlayIndex.count() != 3)
            {
                lastPlayIndex.clear();
                lastPlayIndex << "1" << "-1" << "-1";
            }
        }
        G_SETTING_PTR->setValue(MusicSettingManager::Config::LastPlayIndex, lastPlayIndex);
    }

    G_SETTING_PTR->setValue(MusicSettingManager::Config::LanguageIndex, readXmlAttributeByTagNameValue("language").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::AutoPlayMode, readXmlAttributeByTagNameValue("autoPlayMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::CloseEventMode, readXmlAttributeByTagNameValue("closeEventMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::CloseNetWorkMode, readXmlAttributeByTagNameValue("closeNetworkMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::FileAssociationMode, readXmlAttributeByTagNameValue("fileAssociationMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::WindowConciseMode, readXmlAttributeByTagNameValue("windowConciseMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::RemoteWidgetMode, readXmlAttributeByTagNameValue("remoteWidgetMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::WindowQuitMode, readXmlAttributeByTagNameValue("windowQuitMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LastFileDialogPath, readXmlAttributeByTagNameValue("lastFileDialogPath"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherCheckUpdateEnable, readXmlAttributeByTagNameValue("otherCheckUpdateEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherReadAlbumCover, readXmlAttributeByTagNameValue("otherUseAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherReadFileInfo, readXmlAttributeByTagNameValue("otherUseFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherWriteAlbumCover, readXmlAttributeByTagNameValue("otherWriteAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherWriteFileInfo, readXmlAttributeByTagNameValue("otherWriteFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherSideByMode, readXmlAttributeByTagNameValue("otherSideByMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherLrcKTVMode, readXmlAttributeByTagNameValue("otherLrcKTVMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherScreenSaverEnable, readXmlAttributeByTagNameValue("otherScreenSaverEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherScreenSaverTime, readXmlAttributeByTagNameValue("otherScreenSaverTime").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::OtherScreenSaverIndex, readXmlAttributeByTagNameValue("otherScreenSaverIndex"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::RippleLowPowerMode, readXmlAttributeByTagNameValue("rippleLowPowerMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::RippleSpectrumEnable, readXmlAttributeByTagNameValue("rippleSpectrumEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::RippleSpectrumColor, readXmlAttributeByTagNameValue("rippleSpectrumColor"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::BackgroundThemeValue, readXmlAttributeByTagNameValue("backgroundThemeValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::BackgroundTransparent, readXmlAttributeByTagNameValue("backgroundTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::BackgroundTransparentEnable, readXmlAttributeByTagNameValue("backgroundTransparentEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::BackgroundListTransparent, readXmlAttributeByTagNameValue("backgroundListTransparent").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::Config::HotkeyEnable, readXmlAttributeByTagNameValue("hotkeyEnable"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::HotkeyValue, readXmlAttributeByTagNameValue("hotkeyValue"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::ShowInteriorLrc, readXmlAttributeByTagNameValue("showInteriorLrc").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcColor, readXmlAttributeByTagNameValue("lrcColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcSize, readXmlAttributeByTagNameValue("lrcSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcFamily, readXmlAttributeByTagNameValue("lrcFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcFrontgroundColor, readXmlAttributeByTagNameValue("lrcFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcBackgroundColor, readXmlAttributeByTagNameValue("lrcBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcType, readXmlAttributeByTagNameValue("lrcType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::LrcColorTransparent, readXmlAttributeByTagNameValue("lrcTransparent").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::Config::ShowDesktopLrc, readXmlAttributeByTagNameValue("showDesktopLrc").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcColor, readXmlAttributeByTagNameValue("lrcDColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcSize, readXmlAttributeByTagNameValue("lrcDSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcFamily, readXmlAttributeByTagNameValue("lrcDFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcFrontgroundColor, readXmlAttributeByTagNameValue("lrcDFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcBackgroundColor, readXmlAttributeByTagNameValue("lrcDBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcType, readXmlAttributeByTagNameValue("lrcDType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcColorTransparent, readXmlAttributeByTagNameValue("lrcDTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcWindowMode, readXmlAttributeByTagNameValue("lrcDWindowMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcSingleLineMode, readXmlAttributeByTagNameValue("lrcDSingleLineMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DLrcLockedMode, readXmlAttributeByTagNameValue("lrcDLockedMode").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::Config::EnhancedMusicIndex, readXmlAttributeByTagNameValue("enhancedMusicIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EqualizerIndex, readXmlAttributeByTagNameValue("equalizerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EqualizerEnable, readXmlAttributeByTagNameValue("equalizerEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EqualizerValue, readXmlAttributeByTagNameValue("equalizerValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EnhancedFadeEnable, readXmlAttributeByTagNameValue("enhancedFadeEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EnhancedFadeInValue, readXmlAttributeByTagNameValue("enhancedFadeInValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EnhancedFadeOutValue, readXmlAttributeByTagNameValue("enhancedFadeOutValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::EnhancedEffectValue, readXmlAttributeByTagNameValue("enhancedEffectValue"));

    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoIndex, readXmlAttributeByTagNameValue("timeAutoIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlayMode, readXmlAttributeByTagNameValue("timeAutoPlayMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlayHour, readXmlAttributeByTagNameValue("timeAutoPlayHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlaySecond, readXmlAttributeByTagNameValue("timeAutoPlaySecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlayRepeat, readXmlAttributeByTagNameValue("timeAutoPlayRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlayItemIndex, readXmlAttributeByTagNameValue("timeAutoPlayItemIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoPlaySongIndex, readXmlAttributeByTagNameValue("timeAutoPlaySongIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoStopMode, readXmlAttributeByTagNameValue("timeAutoStopMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoStopHour, readXmlAttributeByTagNameValue("timeAutoStopHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoStopSecond, readXmlAttributeByTagNameValue("timeAutoStopSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoStopRepeat, readXmlAttributeByTagNameValue("timeAutoStopRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoShutdownMode, readXmlAttributeByTagNameValue("timeAutoShutdownMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoShutdownHour, readXmlAttributeByTagNameValue("timeAutoShutdownHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoShutdownSecond, readXmlAttributeByTagNameValue("timeAutoShutdownSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::TimerAutoShutdownRepeat, readXmlAttributeByTagNameValue("timeAutoShutdownRepeat").toInt());

    value = readXmlAttributeByTagNameValue("downloadMusicPath");
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadMusicDirPath, (value.isEmpty() || !QFile::exists(value)) ? MusicUtils::String::musicDirPrefix() : value);
    value = readXmlAttributeByTagNameValue("downloadLrcPath");
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadLrcDirPath, (value.isEmpty() || !QFile::exists(value)) ? MusicUtils::String::lrcDirPrefix() : value);

    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadCacheEnable, readXmlAttributeByTagNameValue("downloadCacheEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadCacheSize, readXmlAttributeByTagNameValue("downloadCacheSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadLimitEnable, readXmlAttributeByTagNameValue("downloadLimitEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadServerIndex, readXmlAttributeByTagNameValue("downloadServerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadDownloadLimitSize, readXmlAttributeByTagNameValue("downloadDownloadLimitSize"));
    G_SETTING_PTR->setValue(MusicSettingManager::Config::DownloadUploadLimitSize, readXmlAttributeByTagNameValue("downloadUploadLimitSize"));
}

void MusicConfigManager::writeBuffer()
{
    const int playMode = G_SETTING_PTR->value(MusicSettingManager::Config::PlayMode).toInt();
    const int volume = G_SETTING_PTR->value(MusicSettingManager::Config::Volume).toInt();
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::Config::LastPlayIndex).toStringList();
    //
    const QPoint &widgetPosition = G_SETTING_PTR->value(MusicSettingManager::Config::WidgetPosition).toPoint();
    const QSize &widgetSize = G_SETTING_PTR->value(MusicSettingManager::Config::WidgetSize).toSize();
    const int autoPlayMode = G_SETTING_PTR->value(MusicSettingManager::Config::AutoPlayMode).toInt();
    const int languageIndex = G_SETTING_PTR->value(MusicSettingManager::Config::LanguageIndex).toInt();
    const int closeEventMode = G_SETTING_PTR->value(MusicSettingManager::Config::CloseEventMode).toInt();
    const int closeNetWorkMode = G_SETTING_PTR->value(MusicSettingManager::Config::CloseNetWorkMode).toInt();
    const int fileAssociationMode = G_SETTING_PTR->value(MusicSettingManager::Config::FileAssociationMode).toInt();
    const int windowConciseMode = G_SETTING_PTR->value(MusicSettingManager::Config::WindowConciseMode).toInt();
    const int remoteWidgetMode = G_SETTING_PTR->value(MusicSettingManager::Config::RemoteWidgetMode).toInt();
    const int windowQuitMode = G_SETTING_PTR->value(MusicSettingManager::Config::WindowQuitMode).toInt();
    const QString lastFileDialogPath = G_SETTING_PTR->value(MusicSettingManager::Config::LastFileDialogPath).toString();
    //
    const int otherCheckUpdateEnable = G_SETTING_PTR->value(MusicSettingManager::Config::OtherCheckUpdateEnable).toInt();
    const int otherReadAlbumCover = G_SETTING_PTR->value(MusicSettingManager::Config::OtherReadAlbumCover).toInt();
    const int otherReadFileInfo = G_SETTING_PTR->value(MusicSettingManager::Config::OtherReadFileInfo).toInt();
    const int otherWriteAlbumCover = G_SETTING_PTR->value(MusicSettingManager::Config::OtherWriteAlbumCover).toInt();
    const int otherWriteFileInfo = G_SETTING_PTR->value(MusicSettingManager::Config::OtherWriteFileInfo).toInt();
    const int otherSideByMode = G_SETTING_PTR->value(MusicSettingManager::Config::OtherSideByMode).toInt();
    const int otherLrcKTVMode = G_SETTING_PTR->value(MusicSettingManager::Config::OtherLrcKTVMode).toInt();
    const int otherScreenSaverEnable = G_SETTING_PTR->value(MusicSettingManager::Config::OtherScreenSaverEnable).toInt();
    const int otherScreenSaverTime = G_SETTING_PTR->value(MusicSettingManager::Config::OtherScreenSaverTime).toInt();
    const QString &otherScreenSaverIndex = G_SETTING_PTR->value(MusicSettingManager::Config::OtherScreenSaverIndex).toString();
    //
    const int rippleLowPowerMode = G_SETTING_PTR->value(MusicSettingManager::Config::RippleLowPowerMode).toInt();
    const int rippleSpectrumEnable = G_SETTING_PTR->value(MusicSettingManager::Config::RippleSpectrumEnable).toInt();
    const QString &rippleSpectrumColor = G_SETTING_PTR->value(MusicSettingManager::Config::RippleSpectrumColor).toString();
    //
    const QString &backgroundThemeValue = G_SETTING_PTR->value(MusicSettingManager::Config::BackgroundThemeValue).toString();
    const int backgroundTransparent = G_SETTING_PTR->value(MusicSettingManager::Config::BackgroundTransparent).toInt();
    const int backgroundListTransparent = G_SETTING_PTR->value(MusicSettingManager::Config::BackgroundListTransparent).toInt();
    const int backgroundTransparentEnable = G_SETTING_PTR->value(MusicSettingManager::Config::BackgroundTransparentEnable).toInt();
    //
    const int hotkeyEnable = G_SETTING_PTR->value(MusicSettingManager::Config::HotkeyEnable).toInt();
    const QString &hotkeyValue = G_SETTING_PTR->value(MusicSettingManager::Config::HotkeyValue).toString();
    //
    const int showInteriorLrc = G_SETTING_PTR->value(MusicSettingManager::Config::ShowInteriorLrc).toInt();
    const int showDesktopLrc = G_SETTING_PTR->value(MusicSettingManager::Config::ShowDesktopLrc).toInt();
    const int lrcColor = G_SETTING_PTR->value(MusicSettingManager::Config::LrcColor).toInt();
    const int lrcSize = G_SETTING_PTR->value(MusicSettingManager::Config::LrcSize).toInt();
    const int lrcType = G_SETTING_PTR->value(MusicSettingManager::Config::LrcType).toInt();
    const int lrcFamily = G_SETTING_PTR->value(MusicSettingManager::Config::LrcFamily).toInt();
    const QString &lrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::Config::LrcFrontgroundColor).toString();
    const QString &lrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::Config::LrcBackgroundColor).toString();
    const int lrcTransparent = G_SETTING_PTR->value(MusicSettingManager::Config::LrcColorTransparent).toInt();
    //
    const int DLrcColor = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcColor).toInt();
    const int DLrcSize = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcSize).toInt();
    const int DLrcType = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcType).toInt();
    const int DLrcFamily = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcFamily).toInt();
    const QString &DLrcFrontgroundColor = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcFrontgroundColor).toString();
    const QString &DLrcBackgroundColor = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcBackgroundColor).toString();
    const int DLrcTransparent = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcColorTransparent).toInt();
    const int DLrcWindowMode= G_SETTING_PTR->value(MusicSettingManager::Config::DLrcWindowMode).toInt();
    const int DLrcSingleLineMode = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcSingleLineMode).toInt();
    const int DLrcLockedMode = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcLockedMode).toInt();
    const QPoint &DLrcGeometry = G_SETTING_PTR->value(MusicSettingManager::Config::DLrcGeometry).toPoint();
    //
    const int enhancedMusicIndex = G_SETTING_PTR->value(MusicSettingManager::Config::EnhancedMusicIndex).toInt();
    const int equalizerEnable = G_SETTING_PTR->value(MusicSettingManager::Config::EqualizerEnable).toInt();
    const QString &equalizerValue = G_SETTING_PTR->value(MusicSettingManager::Config::EqualizerValue).toString();
    const int equalizerIndex = G_SETTING_PTR->value(MusicSettingManager::Config::EqualizerIndex).toInt();
    const int enhancedFadeEnable = G_SETTING_PTR->value(MusicSettingManager::Config::EnhancedFadeEnable).toInt();
    const int enhancedFadeInValue = G_SETTING_PTR->value(MusicSettingManager::Config::EnhancedFadeInValue).toInt();
    const int enhancedFadeOutValue = G_SETTING_PTR->value(MusicSettingManager::Config::EnhancedFadeOutValue).toInt();
    const QString &enhancedEffectValue = G_SETTING_PTR->value(MusicSettingManager::Config::EnhancedEffectValue).toString();
    //
    const int timeAutoIndex = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoIndex).toInt();
    const int timeAutoPlayMode = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlayMode).toInt();
    const int timeAutoPlayHour = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlayHour).toInt();
    const int timeAutoPlaySecond = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlaySecond).toInt();
    const int timeAutoPlayRepeat = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlayRepeat).toInt();
    const int timeAutoPlayItemIndex = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlayItemIndex).toInt();
    const int timeAutoPlaySongIndex = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoPlaySongIndex).toInt();
    const int timeAutoStopMode = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoStopMode).toInt();
    const int timeAutoStopHour = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoStopHour).toInt();
    const int timeAutoStopSecond = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoStopSecond).toInt();
    const int timeAutoStopRepeat = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoStopRepeat).toInt();
    const int timeAutoShutdownMode = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoShutdownMode).toInt();
    const int timeAutoShutdownHour = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoShutdownHour).toInt();
    const int timeAutoShutdownSecond = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoShutdownSecond).toInt();
    const int timeAutoShutdownRepeat = G_SETTING_PTR->value(MusicSettingManager::Config::TimerAutoShutdownRepeat).toInt();
    //
    const QString &downloadMusicPath = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadMusicDirPath).toString();
    const QString &downloadLrcPath = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadLrcDirPath).toString();
    const int downloadCacheEnable = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadCacheEnable).toInt();
    const int downloadCacheSize = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadCacheSize).toInt();
    const int downloadLimitEnable = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadLimitEnable).toInt();
    const int downloadServerIndex = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadServerIndex).toInt();
    const QString &downloadDownloadLimitSize = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadDownloadLimitSize).toString();
    const QString &downloadUploadLimitSize = G_SETTING_PTR->value(MusicSettingManager::Config::DownloadUploadLimitSize).toString();
    //
    if(!toFile(COFIG_PATH_FULL))
    {
        return;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement baseSettingDom = writeDomNode(rootDom, "musicSetting");
    QDomElement plusSettingDom = writeDomNode(rootDom, "plusSetting");
    QDomElement otherSettingDom = writeDomNode(rootDom, "otherSetting");
    QDomElement rippleSettingDom = writeDomNode(rootDom, "rippleSetting");
    QDomElement backgroundSettingDom = writeDomNode(rootDom, "backgroundSetting");
    QDomElement hotkeySettingDom = writeDomNode(rootDom, "hotkeySetting");
    QDomElement interiorLrcSettingDom = writeDomNode(rootDom, "interiorLrcSetting");
    QDomElement desktopLrcSettingDom = writeDomNode(rootDom, "desktopLrcSetting");
    QDomElement equalizerSettingDom = writeDomNode(rootDom, "equalizerSetting");
    QDomElement timeSettingDom = writeDomNode(rootDom, "timeSetting");
    QDomElement downloadSettingDom = writeDomNode(rootDom, "downloadSetting");

    writeDomElement(baseSettingDom, "configVersion", {"value", TTK_CONFIG_VERSION_STR});
    writeDomElement(baseSettingDom, "playMode", {"value", playMode});
    writeDomElement(baseSettingDom, "playVolume", {"value", volume});
    writeDomElement(baseSettingDom, "lastPlayIndex", {"value", QString("%1,%2,%3").arg(lastPlayIndex[0], lastPlayIndex[1], lastPlayIndex[2])});
    //
    writeDomElement(plusSettingDom, "geometry", {"value", QString("%1,%2,%3,%4").arg(widgetPosition.x()).arg(widgetPosition.y()).arg(widgetSize.width()).arg(widgetSize.height())});
    writeDomElement(plusSettingDom, "language", {"value", languageIndex});
    writeDomElement(plusSettingDom, "autoPlayMode", {"value", autoPlayMode});
    writeDomElement(plusSettingDom, "closeEventMode", {"value", closeEventMode});
    writeDomElement(plusSettingDom, "closeNetworkMode", {"value", closeNetWorkMode});
    writeDomElement(plusSettingDom, "fileAssociationMode", {"value", fileAssociationMode});
    writeDomElement(plusSettingDom, "windowConciseMode", {"value", windowConciseMode});
    writeDomElement(plusSettingDom, "remoteWidgetMode", {"value", remoteWidgetMode});
    writeDomElement(plusSettingDom, "windowQuitMode", {"value", windowQuitMode});
    writeDomElement(plusSettingDom, "lastFileDialogPath", {"value", lastFileDialogPath});
    //
    writeDomElement(otherSettingDom, "otherCheckUpdateEnable", {"value", otherCheckUpdateEnable});
    writeDomElement(otherSettingDom, "otherUseAlbumCover", {"value", otherReadAlbumCover});
    writeDomElement(otherSettingDom, "otherUseFileInfo", {"value", otherReadFileInfo});
    writeDomElement(otherSettingDom, "otherWriteAlbumCover", {"value", otherWriteAlbumCover});
    writeDomElement(otherSettingDom, "otherWriteFileInfo", {"value", otherWriteFileInfo});
    writeDomElement(otherSettingDom, "otherSideByMode", {"value", otherSideByMode});
    writeDomElement(otherSettingDom, "otherLrcKTVMode", {"value", otherLrcKTVMode});
    writeDomElement(otherSettingDom, "otherScreenSaverEnable", {"value", otherScreenSaverEnable});
    writeDomElement(otherSettingDom, "otherScreenSaverTime", {"value", otherScreenSaverTime});
    writeDomElement(otherSettingDom, "otherScreenSaverIndex", {"value", otherScreenSaverIndex});
    //
    writeDomElement(rippleSettingDom, "rippleLowPowerMode", {"value", rippleLowPowerMode});
    writeDomElement(rippleSettingDom, "rippleSpectrumEnable", {"value", rippleSpectrumEnable});
    writeDomElement(rippleSettingDom, "rippleSpectrumColor", {"value", rippleSpectrumColor});
    //
    writeDomElement(backgroundSettingDom, "backgroundThemeValue", {"value", backgroundThemeValue});
    writeDomElement(backgroundSettingDom, "backgroundTransparent", {"value", backgroundTransparent});
    writeDomElement(backgroundSettingDom, "backgroundListTransparent", {"value", backgroundListTransparent});
    writeDomElement(backgroundSettingDom, "backgroundTransparentEnable", {"value", backgroundTransparentEnable});
    //
    writeDomElement(hotkeySettingDom, "hotkeyEnable", {"value", hotkeyEnable});
    writeDomElement(hotkeySettingDom, "hotkeyValue", {"value", hotkeyValue});
    //
    writeDomElement(interiorLrcSettingDom, "showInteriorLrc", {"value", showInteriorLrc});
    writeDomElement(interiorLrcSettingDom, "lrcColor", {"value", lrcColor});
    writeDomElement(interiorLrcSettingDom, "lrcSize", {"value", lrcSize});
    writeDomElement(interiorLrcSettingDom, "lrcFamily", {"value", lrcFamily});
    writeDomElement(interiorLrcSettingDom, "lrcType", {"value", lrcType});
    writeDomElement(interiorLrcSettingDom, "lrcTransparent", {"value", lrcTransparent});
    writeDomElement(interiorLrcSettingDom, "lrcFrontgroundColor", {"value", lrcFrontgroundColor});
    writeDomElement(interiorLrcSettingDom, "lrcBackgroundColor", {"value", lrcBackgroundColor});
    //
    writeDomElement(desktopLrcSettingDom, "showDesktopLrc", {"value", showDesktopLrc});
    writeDomElement(desktopLrcSettingDom, "lrcDColor", {"value", DLrcColor});
    writeDomElement(desktopLrcSettingDom, "lrcDSize", {"value", DLrcSize});
    writeDomElement(desktopLrcSettingDom, "lrcDFamily", {"value", DLrcFamily});
    writeDomElement(desktopLrcSettingDom, "lrcDType", {"value", DLrcType});
    writeDomElement(desktopLrcSettingDom, "lrcDTransparent", {"value", DLrcTransparent});
    writeDomElement(desktopLrcSettingDom, "lrcDFrontgroundColor", {"value", DLrcFrontgroundColor});
    writeDomElement(desktopLrcSettingDom, "lrcDBackgroundColor", {"value", DLrcBackgroundColor});
    writeDomElement(desktopLrcSettingDom, "lrcDWindowMode", {"value", DLrcWindowMode});
    writeDomElement(desktopLrcSettingDom, "lrcDSingleLineMode", {"value", DLrcSingleLineMode});
    writeDomElement(desktopLrcSettingDom, "lrcDLockedMode", {"value", DLrcLockedMode});
    writeDomElement(desktopLrcSettingDom, "lrcDGeometry", {"value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())});
    //
    writeDomElement(equalizerSettingDom, "enhancedMusicIndex", {"value", enhancedMusicIndex});
    writeDomElement(equalizerSettingDom, "equalizerEnable", {"value", equalizerEnable});
    writeDomElement(equalizerSettingDom, "equalizerIndex", {"value", equalizerIndex});
    writeDomElement(equalizerSettingDom, "equalizerValue", {"value", equalizerValue});
    writeDomElement(equalizerSettingDom, "enhancedFadeEnable", {"value", enhancedFadeEnable});
    writeDomElement(equalizerSettingDom, "enhancedFadeInValue", {"value", enhancedFadeInValue});
    writeDomElement(equalizerSettingDom, "enhancedFadeOutValue", {"value", enhancedFadeOutValue});
    writeDomElement(equalizerSettingDom, "enhancedEffectValue", {"value", enhancedEffectValue});
    //
    writeDomElement(timeSettingDom, "timeAutoIndex", {"value", timeAutoIndex});
    writeDomElement(timeSettingDom, "timeAutoPlayMode", {"value", timeAutoPlayMode});
    writeDomElement(timeSettingDom, "timeAutoPlayHour", {"value", timeAutoPlayHour});
    writeDomElement(timeSettingDom, "timeAutoPlaySecond", {"value", timeAutoPlaySecond});
    writeDomElement(timeSettingDom, "timeAutoPlayRepeat", {"value", timeAutoPlayRepeat});
    writeDomElement(timeSettingDom, "timeAutoPlayItemIndex", {"value", timeAutoPlayItemIndex});
    writeDomElement(timeSettingDom, "timeAutoPlaySongIndex", {"value", timeAutoPlaySongIndex});
    writeDomElement(timeSettingDom, "timeAutoStopMode", {"value", timeAutoStopMode});
    writeDomElement(timeSettingDom, "timeAutoStopHour", {"value", timeAutoStopHour});
    writeDomElement(timeSettingDom, "timeAutoStopSecond", {"value", timeAutoStopSecond});
    writeDomElement(timeSettingDom, "timeAutoStopRepeat", {"value", timeAutoStopRepeat});
    writeDomElement(timeSettingDom, "timeAutoShutdownMode", {"value", timeAutoShutdownMode});
    writeDomElement(timeSettingDom, "timeAutoShutdownHour", {"value", timeAutoShutdownHour});
    writeDomElement(timeSettingDom, "timeAutoShutdownSecond", {"value", timeAutoShutdownSecond});
    writeDomElement(timeSettingDom, "timeAutoShutdownRepeat", {"value", timeAutoShutdownRepeat});
    //
    writeDomElement(downloadSettingDom, "downloadMusicPath", {"value", downloadMusicPath});
    writeDomElement(downloadSettingDom, "downloadLrcPath", {"value", downloadLrcPath});
    writeDomElement(downloadSettingDom, "downloadCacheEnable", {"value", downloadCacheEnable});
    writeDomElement(downloadSettingDom, "downloadCacheSize", {"value", downloadCacheSize});
    writeDomElement(downloadSettingDom, "downloadLimitEnable", {"value", downloadLimitEnable});
    writeDomElement(downloadSettingDom, "downloadServerIndex", {"value", downloadServerIndex});
    writeDomElement(downloadSettingDom, "downloadDownloadLimitSize", {"value", downloadDownloadLimitSize});
    writeDomElement(downloadSettingDom, "downloadUploadLimitSize", {"value", downloadUploadLimitSize});

    QTextStream out(m_file);
    m_document->save(out, 4);
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
                     list[2].toInt(),
                     list[3].toInt());
    }
    else
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }
}

QPoint MusicConfigManager::readShowDesktopLrcGeometry() const
{
    const QStringList &point = readXmlAttributeByTagNameValue("lrcDGeometry").split(",");
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}
