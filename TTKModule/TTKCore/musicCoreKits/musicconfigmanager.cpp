#include "musicconfigmanager.h"
#include "musicsettingmanager.h"
#include "musicstringutils.h"
#include "ttkversion.h"

#include <QRect>

MusicConfigManager::MusicConfigManager(QObject *parent)
    : TTKXmlDocument(parent)
{

}

bool MusicConfigManager::readBuffer()
{
    int items = 0;
    return readBuffer(items);
}

bool MusicConfigManager::writeBuffer()
{
    int items = 0;
    return writeBuffer(items);
}

bool MusicConfigManager::readBuffer(int &items)
{
    Q_UNUSED(items);

    G_SETTING_PTR->setValue(MusicSettingManager::ConfigVersion, readXmlAttributeByTagName("configVersion"));
    G_SETTING_PTR->setValue(MusicSettingManager::PlayMode, readXmlAttributeByTagName("playMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::Volume, readXmlAttributeByTagName("playVolume").toInt());

    QString value = readXmlAttributeByTagName("lastPlayIndex");
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
        G_SETTING_PTR->setValue(MusicSettingManager::LastPlayIndex, lastPlayIndex);
    }

    G_SETTING_PTR->setValue(MusicSettingManager::LanguageIndex, readXmlAttributeByTagName("language").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::StartUpMode, readXmlAttributeByTagName("startUpMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::StartUpPlayMode, readXmlAttributeByTagName("startUpPlayMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseEventMode, readXmlAttributeByTagName("closeEventMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::CloseNetWorkMode, readXmlAttributeByTagName("closeNetworkMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationMode, readXmlAttributeByTagName("fileAssociationMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::FileAssociationValue, readXmlAttributeByTagName("fileAssociationValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::WindowConciseMode, readXmlAttributeByTagName("windowConciseMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetMode, readXmlAttributeByTagName("remoteWidgetMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::WindowQuitMode, readXmlAttributeByTagName("windowQuitMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LastFileDialogPath, readXmlAttributeByTagName("lastFileDialogPath"));
    G_SETTING_PTR->setValue(MusicSettingManager::MediaLibraryPath, readXmlAttributeByTagName("mediaLibraryPath"));

    G_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateEnable, readXmlAttributeByTagName("otherCheckUpdateEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherReadAlbumCover, readXmlAttributeByTagName("otherUseAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherReadFileInfo, readXmlAttributeByTagName("otherUseFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCover, readXmlAttributeByTagName("otherWriteAlbumCover").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherWriteFileInfo, readXmlAttributeByTagName("otherWriteFileInfo").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherSideByMode, readXmlAttributeByTagName("otherSideByMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVMode, readXmlAttributeByTagName("otherLrcKTVMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverEnable, readXmlAttributeByTagName("otherScreenSaverEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverTime, readXmlAttributeByTagName("otherScreenSaverTime").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::OtherScreenSaverIndex, readXmlAttributeByTagName("otherScreenSaverIndex"));
    G_SETTING_PTR->setValue(MusicSettingManager::OtherPlaylistAutoSaveEnable, readXmlAttributeByTagName("otherPlaylistAutoSaveEnable"));
    G_SETTING_PTR->setValue(MusicSettingManager::OtherRandomShuffleMode, readXmlAttributeByTagName("otherRandomShuffleMode"));

    G_SETTING_PTR->setValue(MusicSettingManager::RippleLowPowerMode, readXmlAttributeByTagName("rippleLowPowerMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumEnable, readXmlAttributeByTagName("rippleSpectrumEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::RippleSpectrumColor, readXmlAttributeByTagName("rippleSpectrumColor"));

    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeValue, readXmlAttributeByTagName("backgroundThemeValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparent, readXmlAttributeByTagName("backgroundTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnable, readXmlAttributeByTagName("backgroundTransparentEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparent, readXmlAttributeByTagName("backgroundListTransparent").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnable, readXmlAttributeByTagName("hotkeyEnable"));
    G_SETTING_PTR->setValue(MusicSettingManager::HotkeyValue, readXmlAttributeByTagName("hotkeyValue"));

    G_SETTING_PTR->setValue(MusicSettingManager::LrcColor, readXmlAttributeByTagName("lrcColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcSize, readXmlAttributeByTagName("lrcSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFamily, readXmlAttributeByTagName("lrcFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColor, readXmlAttributeByTagName("lrcFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColor, readXmlAttributeByTagName("lrcBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::LrcType, readXmlAttributeByTagName("lrcType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransparent, readXmlAttributeByTagName("lrcTransparent").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrc, readXmlAttributeByTagName("showDesktopLrc").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColor, readXmlAttributeByTagName("lrcDColor").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSize, readXmlAttributeByTagName("lrcDSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFamily, readXmlAttributeByTagName("lrcDFamily"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColor, readXmlAttributeByTagName("lrcDFrontgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColor, readXmlAttributeByTagName("lrcDBackgroundColor"));
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcType, readXmlAttributeByTagName("lrcDType").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransparent, readXmlAttributeByTagName("lrcDTransparent").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowMode, readXmlAttributeByTagName("lrcDWindowMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineMode, readXmlAttributeByTagName("lrcDSingleLineMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DLrcLockedMode, readXmlAttributeByTagName("lrcDLockedMode").toInt());

    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicIndex, readXmlAttributeByTagName("enhancedMusicIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndex, readXmlAttributeByTagName("equalizerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnable, readXmlAttributeByTagName("equalizerEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EqualizerValue, readXmlAttributeByTagName("equalizerValue"));
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnable, readXmlAttributeByTagName("enhancedFadeEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValue, readXmlAttributeByTagName("enhancedFadeInValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValue, readXmlAttributeByTagName("enhancedFadeOutValue").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::EnhancedEffectValue, readXmlAttributeByTagName("enhancedEffectValue"));

    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoIndex, readXmlAttributeByTagName("timeAutoIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayMode, readXmlAttributeByTagName("timeAutoPlayMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayHour, readXmlAttributeByTagName("timeAutoPlayHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySecond, readXmlAttributeByTagName("timeAutoPlaySecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayRepeat, readXmlAttributeByTagName("timeAutoPlayRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlayItemIndex, readXmlAttributeByTagName("timeAutoPlayItemIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoPlaySongIndex, readXmlAttributeByTagName("timeAutoPlaySongIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopMode, readXmlAttributeByTagName("timeAutoStopMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopHour, readXmlAttributeByTagName("timeAutoStopHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopSecond, readXmlAttributeByTagName("timeAutoStopSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoStopRepeat, readXmlAttributeByTagName("timeAutoStopRepeat").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownMode, readXmlAttributeByTagName("timeAutoShutdownMode").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownHour, readXmlAttributeByTagName("timeAutoShutdownHour").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownSecond, readXmlAttributeByTagName("timeAutoShutdownSecond").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::TimerAutoShutdownRepeat, readXmlAttributeByTagName("timeAutoShutdownRepeat").toInt());

    value = readXmlAttributeByTagName("downloadMusicPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadMusicDirPath, (value.isEmpty() || !QFile::exists(value)) ? TTK::String::musicDirPrefix() : value);
    value = readXmlAttributeByTagName("downloadLrcPath");
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcDirPath, (value.isEmpty() || !QFile::exists(value)) ? TTK::String::lrcDirPrefix() : value);

    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheEnable, readXmlAttributeByTagName("downloadCacheEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadCacheSize, readXmlAttributeByTagName("downloadCacheSize").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadLimitEnable, readXmlAttributeByTagName("downloadLimitEnable").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadServerIndex, readXmlAttributeByTagName("downloadServerIndex").toInt());
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadDownloadLimitSize, readXmlAttributeByTagName("downloadDownloadLimitSize"));
    G_SETTING_PTR->setValue(MusicSettingManager::DownloadUploadLimitSize, readXmlAttributeByTagName("downloadUploadLimitSize"));

    return true;
}

bool MusicConfigManager::writeBuffer(const int &items)
{
    Q_UNUSED(items);

    const int playMode = G_SETTING_PTR->value(MusicSettingManager::PlayMode).toInt();
    const int volume = G_SETTING_PTR->value(MusicSettingManager::Volume).toInt();
    const QStringList &lastPlayIndex = G_SETTING_PTR->value(MusicSettingManager::LastPlayIndex).toStringList();
    //
    const QPoint &widgetPosition = G_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    const QSize &widgetSize = G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    const int startUpMode = G_SETTING_PTR->value(MusicSettingManager::StartUpMode).toInt();
    const int startUpPlayMode = G_SETTING_PTR->value(MusicSettingManager::StartUpPlayMode).toInt();
    const int languageIndex = G_SETTING_PTR->value(MusicSettingManager::LanguageIndex).toInt();
    const int closeEventMode = G_SETTING_PTR->value(MusicSettingManager::CloseEventMode).toInt();
    const int closeNetWorkMode = G_SETTING_PTR->value(MusicSettingManager::CloseNetWorkMode).toInt();
    const int fileAssociationMode = G_SETTING_PTR->value(MusicSettingManager::FileAssociationMode).toInt();
    const QString &fileAssociationValue = G_SETTING_PTR->value(MusicSettingManager::FileAssociationValue).toString();

    const int windowConciseMode = G_SETTING_PTR->value(MusicSettingManager::WindowConciseMode).toInt();
    const int remoteWidgetMode = G_SETTING_PTR->value(MusicSettingManager::RemoteWidgetMode).toInt();
    const int windowQuitMode = G_SETTING_PTR->value(MusicSettingManager::WindowQuitMode).toInt();
    const QString &lastFileDialogPath = G_SETTING_PTR->value(MusicSettingManager::LastFileDialogPath).toString();
    const QString &mediaLibraryPath = G_SETTING_PTR->value(MusicSettingManager::MediaLibraryPath).toString();
    //
    const int otherCheckUpdateEnable = G_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateEnable).toInt();
    const int otherReadAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherReadAlbumCover).toInt();
    const int otherReadFileInfo = G_SETTING_PTR->value(MusicSettingManager::OtherReadFileInfo).toInt();
    const int otherWriteAlbumCover = G_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCover).toInt();
    const int otherWriteFileInfo = G_SETTING_PTR->value(MusicSettingManager::OtherWriteFileInfo).toInt();
    const int otherSideByMode = G_SETTING_PTR->value(MusicSettingManager::OtherSideByMode).toInt();
    const int otherLrcKTVMode = G_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVMode).toInt();
    const int otherScreenSaverEnable = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverEnable).toInt();
    const int otherScreenSaverTime = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverTime).toInt();
    const QString &otherScreenSaverIndex = G_SETTING_PTR->value(MusicSettingManager::OtherScreenSaverIndex).toString();
    const int otherPlaylistAutoSaveEnable = G_SETTING_PTR->value(MusicSettingManager::OtherPlaylistAutoSaveEnable).toInt();
    const int otherRandomShuffleMode = G_SETTING_PTR->value(MusicSettingManager::OtherRandomShuffleMode).toInt();
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

    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);
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
    writeDomElement(plusSettingDom, "startUpMode", {"value", startUpMode});
    writeDomElement(plusSettingDom, "startUpPlayMode", {"value", startUpPlayMode});
    writeDomElement(plusSettingDom, "closeEventMode", {"value", closeEventMode});
    writeDomElement(plusSettingDom, "closeNetworkMode", {"value", closeNetWorkMode});
    writeDomElement(plusSettingDom, "fileAssociationMode", {"value", fileAssociationMode});
    writeDomElement(plusSettingDom, "fileAssociationValue", {"value", fileAssociationValue});
    writeDomElement(plusSettingDom, "windowConciseMode", {"value", windowConciseMode});
    writeDomElement(plusSettingDom, "remoteWidgetMode", {"value", remoteWidgetMode});
    writeDomElement(plusSettingDom, "windowQuitMode", {"value", windowQuitMode});
    writeDomElement(plusSettingDom, "lastFileDialogPath", {"value", lastFileDialogPath});
    writeDomElement(plusSettingDom, "mediaLibraryPath", {"value", mediaLibraryPath});
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
    writeDomElement(otherSettingDom, "otherPlaylistAutoSaveEnable", {"value", otherPlaylistAutoSaveEnable});
    writeDomElement(otherSettingDom, "otherRandomShuffleMode", {"value", otherRandomShuffleMode});
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

    save();
    return true;
}

QRect MusicConfigManager::readWindowGeometry() const
{
    const QDomNodeList &nodes = m_document->elementsByTagName("geometry");
    if(nodes.isEmpty())
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }

    const QDomElement &element = nodes.item(0).toElement();
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
    const QStringList &point = readXmlAttributeByTagName("lrcDGeometry").split(",");
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}
