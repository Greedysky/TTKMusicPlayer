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
    M_SETTING_PTR->setValue(MusicSettingManager::PlayModeChoiced,
                     readXmlAttributeByTagNameValue("playMode").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::VolumeChoiced,
                     readXmlAttributeByTagNameValue("playVolume").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::CurrentLanIndexChoiced,
                     readXmlAttributeByTagNameValue("language").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::AutoPlayChoiced,
                     readXmlAttributeByTagNameValue("autoPlay").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::CloseEventChoiced,
                     readXmlAttributeByTagNameValue("closeEvent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::CloseNetWorkChoiced,
                     readXmlAttributeByTagNameValue("closeNetwork").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::FileAssociationChoiced,
                     readXmlAttributeByTagNameValue("fileAssociation").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::WindowConciseChoiced,
                     readXmlAttributeByTagNameValue("windowConcise").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::RemoteWidgetModeChoiced,
                     readXmlAttributeByTagNameValue("remoteWidgetMode").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::WindowQuitModeChoiced,
                     readXmlAttributeByTagNameValue("windowQuitMode").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::OtherBackgroundLosslessChoiced,
                     readXmlAttributeByTagNameValue("otherBackgroundLossless").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherCheckUpdateChoiced,
                     readXmlAttributeByTagNameValue("otherCheckUpdate").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSearchChoiced,
                     readXmlAttributeByTagNameValue("otherSearch").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseAlbumCoverChoiced,
                     readXmlAttributeByTagNameValue("otherUseAlbumCover").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherUseInfoChoiced,
                     readXmlAttributeByTagNameValue("otherUseInfo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteAlbumCoverChoiced,
                     readXmlAttributeByTagNameValue("otherWriteAlbumCover").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherWriteInfoChoiced,
                     readXmlAttributeByTagNameValue("otherWriteInfo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSideByChoiced,
                     readXmlAttributeByTagNameValue("otherSideBy").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherSongFormat,
                     readXmlAttributeByTagNameValue("otherSongFormat").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherLrcKTVModeChoiced,
                     readXmlAttributeByTagNameValue("otherLrcKTV").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumEnableChoiced,
                     readXmlAttributeByTagNameValue("otherRippleSpectrumEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumColorChoiced,
                     readXmlAttributeByTagNameValue("otherRippleSpectrumColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::OtherRippleSpectrumOpacityChoiced,
                     readXmlAttributeByTagNameValue("otherRippleSpectrumOpacity").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundThemeChoiced,
                     readXmlAttributeByTagNameValue("backgroundTheme"));
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentChoiced,
                     readXmlAttributeByTagNameValue("backgroundTransparent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundTransparentEnableChoiced,
                     readXmlAttributeByTagNameValue("backgroundTransparentEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::BackgroundListTransparentChoiced,
                     readXmlAttributeByTagNameValue("backgroundListTransparent").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyEnableChoiced,
                     readXmlAttributeByTagNameValue("hotkeyEnable"));
    M_SETTING_PTR->setValue(MusicSettingManager::HotkeyStringChoiced,
                     readXmlAttributeByTagNameValue("hotkeyString"));


    M_SETTING_PTR->setValue(MusicSettingManager::ShowInlineLrcChoiced,
                     readXmlAttributeByTagNameValue("showInlineLrc").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorChoiced,
                     readXmlAttributeByTagNameValue("lrcColor").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcSizeChoiced,
                     readXmlAttributeByTagNameValue("lrcSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFamilyChoiced,
                     readXmlAttributeByTagNameValue("lrcFamily"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcFrontgroundColorChoiced,
                     readXmlAttributeByTagNameValue("lrcFrontgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcBackgroundColorChoiced,
                     readXmlAttributeByTagNameValue("lrcBackgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::LrcTypeChoiced,
                     readXmlAttributeByTagNameValue("lrcType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::LrcColorTransChoiced,
                     readXmlAttributeByTagNameValue("lrcTransparent").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::ShowDesktopLrcChoiced,
                     readXmlAttributeByTagNameValue("showDesktopLrc").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorChoiced,
                     readXmlAttributeByTagNameValue("lrcDColor").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSizeChoiced,
                     readXmlAttributeByTagNameValue("lrcDSize").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFamilyChoiced,
                     readXmlAttributeByTagNameValue("lrcDFamily"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcFrontgroundColorChoiced,
                     readXmlAttributeByTagNameValue("lrcDFrontgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcBackgroundColorChoiced,
                     readXmlAttributeByTagNameValue("lrcDBackgroundColor"));
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcTypeChoiced,
                     readXmlAttributeByTagNameValue("lrcDType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcColorTransChoiced,
                     readXmlAttributeByTagNameValue("lrcDTransparent").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcWindowTypeChoiced,
                     readXmlAttributeByTagNameValue("lrcDWindowType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcSingleLineTypeChoiced,
                     readXmlAttributeByTagNameValue("lrcDSingleLineType").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::DLrcLockedChoiced,
                     readXmlAttributeByTagNameValue("lrcDLocked").toInt());


    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedMusicChoiced,
                     readXmlAttributeByTagNameValue("enhancedMusic").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerIndexChoiced,
                     readXmlAttributeByTagNameValue("equalizerIndex").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerEnableChoiced,
                     readXmlAttributeByTagNameValue("equalizerEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EqualizerValueChoiced,
                     readXmlAttributeByTagNameValue("equalizerValue"));
//    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBalanceChoiced,
//                     readXmlAttributeByTagNameValue("enhancedBalance").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBalanceChoiced, 0);
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeEnableChoiced,
                     readXmlAttributeByTagNameValue("enhancedFadeEnable").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeInValueChoiced,
                     readXmlAttributeByTagNameValue("enhancedFadeInValue").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedFadeOutValueChoiced,
                     readXmlAttributeByTagNameValue("enhancedFadeOutValue").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedBS2BChoiced,
                     readXmlAttributeByTagNameValue("enhancedBS2B").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedCrossfadeChoiced,
                     readXmlAttributeByTagNameValue("enhancedCrossfade").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedStereoChoiced,
                     readXmlAttributeByTagNameValue("enhancedStereo").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedLADSPAChoiced,
                     readXmlAttributeByTagNameValue("enhancedLADSPA").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSOXChoiced,
                     readXmlAttributeByTagNameValue("enhancedSOX").toInt());
    M_SETTING_PTR->setValue(MusicSettingManager::EnhancedSRCChoiced,
                     readXmlAttributeByTagNameValue("enhancedSRC").toInt());


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
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::musicPrefix() : path);
    path = readXmlAttributeByTagNameValue("downloadLrcPath");
    M_SETTING_PTR->setValue(MusicSettingManager::DownloadLrcPathDirChoiced,
                           (path.isEmpty() || !QFile::exists(path)) ? MusicUtils::String::lrcPrefix() : path);
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


    M_SETTING_PTR->setValue(MusicSettingManager::QiNiuMusicConfigChoiced,
                     readXmlAttributeByTagNameValue("qiniuMusicConfig"));
    M_SETTING_PTR->setValue(MusicSettingManager::QiNiuDataConfigChoiced,
                     readXmlAttributeByTagNameValue("qiniuDataConfig"));
    M_SETTING_PTR->setValue(MusicSettingManager::QiNiuTimeConfigChoiced,
                     readXmlAttributeByTagNameValue("qiniuTimeConfig"));
}

void MusicSysConfigManager::writeSysConfigData()
{
    const int playModeChoiced = M_SETTING_PTR->value(MusicSettingManager::PlayModeChoiced).toInt();
    const int volumeChoiced = M_SETTING_PTR->value(MusicSettingManager::VolumeChoiced).toInt();
    const QStringList &lastPlayIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::LastPlayIndexChoiced).toStringList();

    //
    const QPoint &widgetPositionChoiced = M_SETTING_PTR->value(MusicSettingManager::WidgetPosition).toPoint();
    const QSize &widgetSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize();
    const int autoPlayChoiced = M_SETTING_PTR->value(MusicSettingManager::AutoPlayChoiced).toInt();
    const int languageIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::CurrentLanIndexChoiced).toInt();
    const int closeEventChoiced = M_SETTING_PTR->value(MusicSettingManager::CloseEventChoiced).toInt();
    const int closeNetWorkChoiced = M_SETTING_PTR->value(MusicSettingManager::CloseNetWorkChoiced).toInt();
    const int fileAssociationChoiced = M_SETTING_PTR->value(MusicSettingManager::FileAssociationChoiced).toInt();
    const int windowConciseChoiced = M_SETTING_PTR->value(MusicSettingManager::WindowConciseChoiced).toInt();
    const int remoteWidgetModeChoiced = M_SETTING_PTR->value(MusicSettingManager::RemoteWidgetModeChoiced).toInt();
    const int windowQuitModeChoiced = M_SETTING_PTR->value(MusicSettingManager::WindowQuitModeChoiced).toInt();

    //
    const int otherBackgroundLosslessChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherBackgroundLosslessChoiced).toInt();
    const int otherCheckUpdateChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherCheckUpdateChoiced).toInt();
    const int otherSearchChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherSearchChoiced).toInt();
    const int otherUseAlbumCoverChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherUseAlbumCoverChoiced).toInt();
    const int otherUseInfoChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherUseInfoChoiced).toInt();
    const int otherWriteAlbumCoverChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherWriteAlbumCoverChoiced).toInt();
    const int otherWriteInfoChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherWriteInfoChoiced).toInt();
    const int otherSideByChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherSideByChoiced).toInt();
    const int otherSongFormatChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherSongFormat).toInt();
    const int otherLrcKTVModeChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherLrcKTVModeChoiced).toInt();
    const int otherRippleSpectrumEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumEnableChoiced).toInt();
    const QString &otherRippleSpectrumColorChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumColorChoiced).toString();
    const int otherRippleSpectrumOpacityChoiced = M_SETTING_PTR->value(MusicSettingManager::OtherRippleSpectrumOpacityChoiced).toInt();

    //
    const QString &backgroundThemeChoiced = M_SETTING_PTR->value(MusicSettingManager::BackgroundThemeChoiced).toString();
    const int backgroundTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentChoiced).toInt();
    const int backgroundListTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::BackgroundListTransparentChoiced).toInt();
    const int backgroundTransparentEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::BackgroundTransparentEnableChoiced).toInt();

    //
    const int hotkeyEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::HotkeyEnableChoiced).toInt();
    const QString &hotkeyStringChoiced = M_SETTING_PTR->value(MusicSettingManager::HotkeyStringChoiced).toString();

    //
    const int showInlineLrcChoiced = M_SETTING_PTR->value(MusicSettingManager::ShowInlineLrcChoiced).toInt();
    const int showDesktopLrcChoiced = M_SETTING_PTR->value(MusicSettingManager::ShowDesktopLrcChoiced).toInt();
    const int lrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorChoiced).toInt();
    const int lrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcSizeChoiced).toInt();
    const int lrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcTypeChoiced).toInt();
    const int lrcFamilyChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFamilyChoiced).toInt();
    const QString &lrcFrontgroundColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcFrontgroundColorChoiced).toString();
    const QString &lrcBackgroundColorChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcBackgroundColorChoiced).toString();
    const int lrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::LrcColorTransChoiced).toInt();

    //
    const int DLrcColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorChoiced).toInt();
    const int DLrcSizeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcSizeChoiced).toInt();
    const int DLrcTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcTypeChoiced).toInt();
    const int DLrcFamilyChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFamilyChoiced).toInt();
    const QString &DLrcFrontgroundColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcFrontgroundColorChoiced).toString();
    const QString &DLrcBackgroundColorChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcBackgroundColorChoiced).toString();
    const int DLrcTransparentChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcColorTransChoiced).toInt();
    const int DLrcWindowTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcWindowTypeChoiced).toInt();
    const int DLrcSingleLineTypeChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcSingleLineTypeChoiced).toInt();
    const int DLrcLockedChoiced = M_SETTING_PTR->value(MusicSettingManager::DLrcLockedChoiced).toInt();
    const QPoint &DLrcGeometry = M_SETTING_PTR->value(MusicSettingManager::DLrcGeometryChoiced).toPoint();

    //
    const int enhancedMusicChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedMusicChoiced).toInt();
    const int equalizerEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerEnableChoiced).toInt();
    const QString &equalizerValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerValueChoiced).toString();
    const int equalizerIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::EqualizerIndexChoiced).toInt();
    const int enhancedBalanceChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedBalanceChoiced).toInt();
    const int enhancedFadeEnableChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeEnableChoiced).toInt();
    const int enhancedFadeInValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeInValueChoiced).toInt();
    const int enhancedFadeOutValueChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedFadeOutValueChoiced).toInt();
    const int enhancedBS2BChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedBS2BChoiced).toInt();
    const int enhancedCrossfadeChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedCrossfadeChoiced).toInt();
    const int enhancedStereoChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedStereoChoiced).toInt();
    const int enhancedLADSPAChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedLADSPAChoiced).toInt();
    const int enhancedSOXChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedSOXChoiced).toInt();
    const int enhancedSRCChoiced = M_SETTING_PTR->value(MusicSettingManager::EnhancedSRCChoiced).toInt();

    //
    const int timeAutoIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoIndexChoiced).toInt();
    const int timeAutoPlayChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayChoiced).toInt();
    const int timeAutoPlayHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayHourChoiced).toInt();
    const int timeAutoPlaySecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySecondChoiced).toInt();
    const int timeAutoPlayRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayRepeatChoiced).toInt();
    const int timeAutoPlayItemIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlayItemIndexChoiced).toInt();
    const int timeAutoPlaySongIndexChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoPlaySongIndexChoiced).toInt();
    const int timeAutoStopChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopChoiced).toInt();
    const int timeAutoStopHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopHourChoiced).toInt();
    const int timeAutoStopSecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopSecondChoiced).toInt();
    const int timeAutoStopRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoStopRepeatChoiced).toInt();
    const int timeAutoShutdownChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownChoiced).toInt();
    const int timeAutoShutdownHourChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownHourChoiced).toInt();
    const int timeAutoShutdownSecondChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownSecondChoiced).toInt();
    const int timeAutoShutdownRepeatChoiced = M_SETTING_PTR->value(MusicSettingManager::TimerAutoShutdownRepeatChoiced).toInt();
    //

    const QString &downloadMusicPath = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    const QString &downloadLrcPath = M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString();
    const int downloadCacheLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheLimitChoiced).toInt();
    const int downloadCacheSize = M_SETTING_PTR->value(MusicSettingManager::DownloadCacheSizeChoiced).toInt();
    const int downloadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadLimitChoiced).toInt();
    const int downloadServer = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    const QString &downloadDLoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadDLoadLimitChoiced).toString();
    const QString &downloadULoadLimit = M_SETTING_PTR->value(MusicSettingManager::DownloadULoadLimitChoiced).toString();
    //

    const QString &qiniuMusicConfig = M_SETTING_PTR->value(MusicSettingManager::QiNiuMusicConfigChoiced).toString();
    const QString &qiniuDataConfig = M_SETTING_PTR->value(MusicSettingManager::QiNiuDataConfigChoiced).toString();
    const QString &qiniuTimeConfig = M_SETTING_PTR->value(MusicSettingManager::QiNiuTimeConfigChoiced).toString();
    //
    if(!writeConfig(COFIGPATH_FULL))
    {
        return;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayerDom = createRoot(APP_NAME);
    //Class A
    QDomElement musicSettingDom = writeDomNode(musicPlayerDom, "musicSetting");
    QDomElement plusSettingDom = writeDomNode(musicPlayerDom, "plusSetting");
    QDomElement otherSettingDom = writeDomNode(musicPlayerDom, "otherSetting");
    QDomElement backgroundSettingDom = writeDomNode(musicPlayerDom, "backgroundSetting");
    QDomElement hotkeySettingDom = writeDomNode(musicPlayerDom, "hotkeySetting");
    QDomElement inlineLrcSettingDom = writeDomNode(musicPlayerDom, "inlineLrcSetting");
    QDomElement desktopLrcSettingDom = writeDomNode(musicPlayerDom, "desktopLrcSetting");
    QDomElement equalizerSettingDom = writeDomNode(musicPlayerDom, "equalizerSetting");
    QDomElement timeSettingDom = writeDomNode(musicPlayerDom, "timeSetting");
    QDomElement downloadSettingDom = writeDomNode(musicPlayerDom, "downloadSetting");
    QDomElement qiniuSettingDom = writeDomNode(musicPlayerDom, "qiniuSetting");
    //Class B
    writeDomElement(musicSettingDom, "playMode", MusicXmlAttribute("value", playModeChoiced));
    writeDomElement(musicSettingDom, "playVolume", MusicXmlAttribute("value", volumeChoiced));
    writeDomElementText(musicSettingDom, "lastPlayIndex", MusicXmlAttribute("value", lastPlayIndexChoiced[0]),
                        QString("%1,%2").arg(lastPlayIndexChoiced[1]).arg(lastPlayIndexChoiced[2]));

    //
    writeDomElement(plusSettingDom, "geometry", MusicXmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height())));
    writeDomElement(plusSettingDom, "language", MusicXmlAttribute("value", languageIndexChoiced));
    writeDomElement(plusSettingDom, "autoPlay", MusicXmlAttribute("value", autoPlayChoiced));
    writeDomElement(plusSettingDom, "closeEvent", MusicXmlAttribute("value", closeEventChoiced));
    writeDomElement(plusSettingDom, "closeNetwork", MusicXmlAttribute("value", closeNetWorkChoiced));
    writeDomElement(plusSettingDom, "fileAssociation", MusicXmlAttribute("value", fileAssociationChoiced));
    writeDomElement(plusSettingDom, "windowConcise", MusicXmlAttribute("value", windowConciseChoiced));
    writeDomElement(plusSettingDom, "remoteWidgetMode", MusicXmlAttribute("value", remoteWidgetModeChoiced));
    writeDomElement(plusSettingDom, "windowQuitMode", MusicXmlAttribute("value", windowQuitModeChoiced));

    //
    writeDomElement(otherSettingDom, "otherBackgroundLossless", MusicXmlAttribute("value", otherBackgroundLosslessChoiced));
    writeDomElement(otherSettingDom, "otherUpdate", MusicXmlAttribute("value", otherCheckUpdateChoiced));
    writeDomElement(otherSettingDom, "otherSearch", MusicXmlAttribute("value", otherSearchChoiced));
    writeDomElement(otherSettingDom, "otherUseAlbumCover", MusicXmlAttribute("value", otherUseAlbumCoverChoiced));
    writeDomElement(otherSettingDom, "otherUseInfo", MusicXmlAttribute("value", otherUseInfoChoiced));
    writeDomElement(otherSettingDom, "otherWriteAlbumCover", MusicXmlAttribute("value", otherWriteAlbumCoverChoiced));
    writeDomElement(otherSettingDom, "otherWriteInfo", MusicXmlAttribute("value", otherWriteInfoChoiced));
    writeDomElement(otherSettingDom, "otherSideBy", MusicXmlAttribute("value", otherSideByChoiced));
    writeDomElement(otherSettingDom, "otherSongFormat", MusicXmlAttribute("value", otherSongFormatChoiced));
    writeDomElement(otherSettingDom, "otherLrcKTV", MusicXmlAttribute("value", otherLrcKTVModeChoiced));
    writeDomElement(otherSettingDom, "otherRippleSpectrumEnable", MusicXmlAttribute("value", otherRippleSpectrumEnableChoiced));
    writeDomElement(otherSettingDom, "otherRippleSpectrumColor", MusicXmlAttribute("value", otherRippleSpectrumColorChoiced));
    writeDomElement(otherSettingDom, "otherRippleSpectrumOpacity", MusicXmlAttribute("value", otherRippleSpectrumOpacityChoiced));

    //
    writeDomElement(backgroundSettingDom, "backgroundTheme", MusicXmlAttribute("value", backgroundThemeChoiced));
    writeDomElement(backgroundSettingDom, "backgroundTransparent", MusicXmlAttribute("value", backgroundTransparentChoiced));
    writeDomElement(backgroundSettingDom, "backgroundListTransparent", MusicXmlAttribute("value", backgroundListTransparentChoiced));
    writeDomElement(backgroundSettingDom, "backgroundTransparentEnable", MusicXmlAttribute("value", backgroundTransparentEnableChoiced));

    //
    writeDomElement(hotkeySettingDom, "hotkeyEnable", MusicXmlAttribute("value", hotkeyEnableChoiced));
    writeDomElement(hotkeySettingDom, "hotkeyString", MusicXmlAttribute("value", hotkeyStringChoiced));

    //
    writeDomElement(inlineLrcSettingDom, "showInlineLrc", MusicXmlAttribute("value", showInlineLrcChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcColor", MusicXmlAttribute("value", lrcColorChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcSize", MusicXmlAttribute("value", lrcSizeChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcFamily", MusicXmlAttribute("value", lrcFamilyChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcType", MusicXmlAttribute("value", lrcTypeChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcTransparent", MusicXmlAttribute("value", lrcTransparentChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcFrontgroundColor", MusicXmlAttribute("value", lrcFrontgroundColorChoiced));
    writeDomElement(inlineLrcSettingDom, "lrcBackgroundColor", MusicXmlAttribute("value", lrcBackgroundColorChoiced));

    //
    writeDomElement(desktopLrcSettingDom, "showDesktopLrc", MusicXmlAttribute("value", showDesktopLrcChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDColor", MusicXmlAttribute("value", DLrcColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDSize", MusicXmlAttribute("value", DLrcSizeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDFamily", MusicXmlAttribute("value", DLrcFamilyChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDType", MusicXmlAttribute("value", DLrcTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDTransparent", MusicXmlAttribute("value", DLrcTransparentChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDFrontgroundColor", MusicXmlAttribute("value", DLrcFrontgroundColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDBackgroundColor", MusicXmlAttribute("value", DLrcBackgroundColorChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDWindowType", MusicXmlAttribute("value", DLrcWindowTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDSingleLineType", MusicXmlAttribute("value", DLrcSingleLineTypeChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDLocked", MusicXmlAttribute("value", DLrcLockedChoiced));
    writeDomElement(desktopLrcSettingDom, "lrcDGeometry", MusicXmlAttribute("value", QString("%1,%2").arg(DLrcGeometry.x()).arg(DLrcGeometry.y())));

    //
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
    writeDomElement(equalizerSettingDom, "enhancedSRC", MusicXmlAttribute("value", enhancedSRCChoiced));

    //
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

    //
    writeDomElement(downloadSettingDom, "downloadMusicPath", MusicXmlAttribute("value", downloadMusicPath));
    writeDomElement(downloadSettingDom, "downloadLrcPath", MusicXmlAttribute("value", downloadLrcPath));
    writeDomElement(downloadSettingDom, "downloadCacheLimit", MusicXmlAttribute("value", downloadCacheLimit));
    writeDomElement(downloadSettingDom, "downloadCacheSize", MusicXmlAttribute("value", downloadCacheSize));
    writeDomElement(downloadSettingDom, "downloadLimit", MusicXmlAttribute("value", downloadLimit));
    writeDomElement(downloadSettingDom, "downloadServer", MusicXmlAttribute("value", downloadServer));
    writeDomElement(downloadSettingDom, "downloadDLoadLimit", MusicXmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloadSettingDom, "downloadULoadLimit", MusicXmlAttribute("value", downloadULoadLimit));

    //
    writeDomElement(qiniuSettingDom, "qiniuMusicConfig", MusicXmlAttribute("value", qiniuMusicConfig));
    writeDomElement(qiniuSettingDom, "qiniuDataConfig", MusicXmlAttribute("value", qiniuDataConfig));
    writeDomElement(qiniuSettingDom, "qiniuTimeConfig", MusicXmlAttribute("value", qiniuTimeConfig));

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
    key << element.attribute("value") << element.text().split(',');
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
    const QStringList &point = readXmlAttributeByTagNameValue("lrcDGeometry").split(',');
    if(point.count() != 2)
    {
        return QPoint();
    }
    return QPoint(point[0].toInt(), point[1].toInt());
}
