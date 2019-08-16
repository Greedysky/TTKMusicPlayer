#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QMetaEnum>
#include "musicobject.h"
#include "musicsingleton.h"

#define M_SETTING_PTR (MusicSingleton<MusicSettingManager>::createInstance())

/*! @brief The class of the paramater setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSettingManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConfigType)
    TTK_DECLARE_MODULE(MusicSettingManager)
public:
    enum ConfigType
    {
        Null = -1,                              /*!< No Parameter*/
        ScreenSize,                             /*!< Screen Size Parameter*/
        WidgetPosition,                         /*!< Widget Position Parameter*/
        WidgetSize,                             /*!< Widget Size Parameter*/

        PlayModeChoiced,                        /*!< Play Mode Parameter*/
        VolumeChoiced,                          /*!< Volume Parameter*/

        AutoPlayChoiced,                        /*!< Auto Play Parameter*/
        CurrentLanIndexChoiced,                 /*!< Current LanIndex Parameter*/
        CloseEventChoiced,                      /*!< Close Event Parameter*/
        LastPlayIndexChoiced,                   /*!< Last Play Index Parameter*/
        CloseNetWorkChoiced,                    /*!< Close NetWork Parameter*/
        FileAssociationChoiced,                 /*!< File Association Parameter*/
        ExtraDevicePathChoiced,                 /*!< Set Extra Device Path Parameter*/
        WindowConciseChoiced,                   /*!< Window Concise Parameter*/
        RemoteWidgetModeChoiced,                /*!< Remote Widget Mode Parameter*/
        WindowQuitModeChoiced,                  /*!< Window Quit Mode Parameter*/

        BackgroundThemeChoiced,                 /*!< Background Theme Parameter*/
        BackgroundTransparentChoiced,           /*!< Background Transparent Parameter*/
        BackgroundListTransparentChoiced,       /*!< Background List Transparent Parameter*/
        BackgroundTransparentEnableChoiced,     /*!< Background Transparent Enable Parameter*/

        OtherBackgroundLosslessChoiced,         /*!< Other Background Lossless Parameter*/
        OtherCheckUpdateChoiced,                /*!< Other Check Update Parameter*/
        OtherSearchChoiced,                     /*!< Other Search Optimzied Parameter*/
        OtherUseAlbumCoverChoiced,              /*!< Other Use File Album Parameter*/
        OtherUseInfoChoiced,                    /*!< Other Use File Info Parameter*/
        OtherWriteAlbumCoverChoiced,            /*!< Other Write File Album Parameter*/
        OtherWriteInfoChoiced,                  /*!< Other Write File Info Parameter*/
        OtherSongFormat,                        /*!< Other Song Format Parameter*/
        OtherSideByChoiced,                     /*!< Other App Side By Mode Parameter*/
        OtherSideByInChoiced,                   /*!< Other App Side By In Mode Parameter*/
        OtherLrcKTVModeChoiced,                 /*!< Other Lrc KTV Mode Mode Parameter*/
        OtherRippleSpectrumEnableChoiced,       /*!< Other Ripple Spectrum Enable Parameter*/
        OtherRippleSpectrumColorChoiced,        /*!< Other Ripple Spectrum Color Parameter*/
        OtherRippleSpectrumOpacityChoiced,      /*!< Other Ripple Spectrum Opacity Parameter*/

        ShowInlineLrcChoiced,                   /*!< Show Inline Lrc Parameter*/
        LrcColorChoiced,                        /*!< Lrc Color Parameter*/
        LrcSizeChoiced,                         /*!< Lrc Size Parameter*/
        LrcTypeChoiced,                         /*!< Lrc Type Parameter*/
        LrcFamilyChoiced,                       /*!< Lrc Family Parameter*/
        LrcFrontgroundColorChoiced,             /*!< Lrc Frontground Color Parameter*/
        LrcBackgroundColorChoiced,              /*!< Lrc Background Color Parameter*/
        LrcColorTransChoiced,                   /*!< Lrc Color Trans Parameter*/

        ShowDesktopLrcChoiced,                  /*!< Show Desktop Lrc Parameter*/
        DLrcColorChoiced,                       /*!< Desktop Lrc Color Parameter*/
        DLrcSizeChoiced,                        /*!< Desktop Lrc Size Parameter*/
        DLrcTypeChoiced,                        /*!< Desktop Lrc Type Parameter*/
        DLrcFamilyChoiced,                      /*!< Desktop Lrc Family Parameter*/
        DLrcFrontgroundColorChoiced,            /*!< Desktop Lrc Frontground Color Parameter*/
        DLrcBackgroundColorChoiced,             /*!< Desktop Lrc Background Color Parameter*/
        DLrcColorTransChoiced,                  /*!< Desktop Lrc Color Trans Parameter*/
        DLrcWindowTypeChoiced,                  /*!< Desktop Lrc Window Type Parameter*/
        DLrcSingleLineTypeChoiced,              /*!< Desktop Lrc Single Line Type Parameter*/
        DLrcLockedChoiced,                      /*!< Desktop Lrc Locked Parameter*/
        DLrcGeometryChoiced,                    /*!< Desktop Lrc Geometry Parameter*/

        EqualizerEnableChoiced,                 /*!< Equalizer Enable Parameter*/
        EqualizerValueChoiced,                  /*!< Equalizer Value Parameter*/
        EqualizerIndexChoiced,                  /*!< Equalizer Index Parameter*/
        EnhancedMusicChoiced,                   /*!< Enhanced Music Parameter*/
        EnhancedBalanceChoiced,                 /*!< Enhanced Balance Parameter*/
        EnhancedFadeEnableChoiced,              /*!< Enhanced Fade Enable Parameter*/
        EnhancedFadeInValueChoiced,             /*!< Enhanced Fade In Value Parameter*/
        EnhancedFadeOutValueChoiced,            /*!< Enhanced Fade Out Value Parameter*/
        EnhancedBS2BChoiced,                    /*!< Enhanced BS2B Parameter*/
        EnhancedCrossfadeChoiced,               /*!< Enhanced Crossfade Parameter*/
        EnhancedStereoChoiced,                  /*!< Enhanced Stereo Parameter*/
        EnhancedLADSPAChoiced,                  /*!< Enhanced LADSPA Parameter*/
        EnhancedSOXChoiced,                     /*!< Enhanced SOX Parameter*/
        EnhancedSRCChoiced,                     /*!< Enhanced SRC Parameter*/

        TimerAutoIndexChoiced,                  /*!< Timer Auto Index Parameter*/
        TimerAutoPlayChoiced,                   /*!< Timer Auto Play Parameter*/
        TimerAutoPlayHourChoiced,               /*!< Timer Auto Play Hour Parameter*/
        TimerAutoPlaySecondChoiced,             /*!< Timer Auto Play Second Parameter*/
        TimerAutoPlayRepeatChoiced,             /*!< Timer Auto Play Repeat Parameter*/
        TimerAutoPlayItemIndexChoiced,          /*!< Timer Auto Play Item Index Parameter*/
        TimerAutoPlaySongIndexChoiced,          /*!< Timer Auto Play Song Index Parameter*/
        TimerAutoStopChoiced,                   /*!< Timer Auto Stop Parameter*/
        TimerAutoStopHourChoiced,               /*!< Timer Auto Stop Hour Parameter*/
        TimerAutoStopSecondChoiced,             /*!< Timer Auto Stop Second Parameter*/
        TimerAutoStopRepeatChoiced,             /*!< Timer Auto Stop Repeat Parameter*/
        TimerAutoShutdownChoiced,               /*!< Timer Auto Shutdown Parameter*/
        TimerAutoShutdownHourChoiced,           /*!< Timer Auto Shutdown Hour Parameter*/
        TimerAutoShutdownSecondChoiced,         /*!< Timer Auto Shutdown Second Parameter*/
        TimerAutoShutdownRepeatChoiced,         /*!< Timer Auto Shutdown Repeat Parameter*/

        DownloadMusicPathDirChoiced,            /*!< Download Music Path Dir Parameter*/
        DownloadLrcPathDirChoiced,              /*!< Download Lrc Path Dir Parameter*/
        DownloadCacheLimitChoiced,              /*!< Download Cache Limit Parameter*/
        DownloadCacheSizeChoiced,               /*!< Download Cache Size Parameter*/
        DownloadLimitChoiced,                   /*!< Download Limit Parameter*/
        DownloadServerChoiced,                  /*!< Download Server Parameter*/
        DownloadDLoadLimitChoiced,              /*!< Download DLoad Limit Parameter*/
        DownloadULoadLimitChoiced,              /*!< Download ULoad Limit Parameter*/

        HotkeyEnableChoiced,                    /*!< Hotkey Enable Parameter*/
        HotkeyStringChoiced,                    /*!< Hotkey String Parameter*/

        QiNiuMusicConfigChoiced,                /*!< QiNiu Music Config Parameter*/
        QiNiuDataConfigChoiced,                 /*!< QiNiu Data Config Parameter*/
        QiNiuTimeConfigChoiced,                 /*!< QiNiu Time Config Parameter*/

        NetworkCookieChoiced                    /*!< Network cookie Parameter*/

#ifdef MUSIC_MOBILE
        ,MobileWifiConnectChoiced               /*!< Mobile Wifi Connect Parameter*/
#endif

    };

    /*!
     * Set current value by Config Type.
     */
    inline void setValue(ConfigType type, const QVariant &var)
    {
        m_para[type] = var;
    }

    /*!
     * Set current value by String Type.
     */
    inline void setValue(const QString &stype, const QVariant &var)
    {
        m_para[typeStringToEnum(stype)] = var;
    }

    /*!
     * Get current value by Config Type.
     */
    inline QVariant value(ConfigType type) const
    {
        return m_para[type];
    }

    /*!
     * Get current value by String Type.
     */
    inline QVariant value(const QString &stype) const
    {
        return m_para[typeStringToEnum(stype)];
    }

    /*!
     * Get parameter count.
     */
    inline int count() const
    {
        return m_para.count();
    }

    /*!
     * Current parameter is empty.
     */
    inline bool isEmpty() const
    {
        return m_para.isEmpty();
    }

    /*!
     * Current parameter contains type.
     */
    inline bool contains(ConfigType type) const
    {
        return m_para.contains(type);
    }

protected:
    /*!
     * Convert String type to Config Type.
     */
    ConfigType typeStringToEnum(const QString &stype) const
    {
        int index = staticMetaObject.indexOfEnumerator("ConfigType");
        QMetaEnum metaEnum = staticMetaObject.enumerator(index);
        int key = metaEnum.keyToValue(stype.toStdString().c_str());
        return MStatic_cast(ConfigType, key);
    }

    QVariant m_variant;
    QMap<ConfigType, QVariant> m_para;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};


#endif // MUSICSETTINGMANAGER

