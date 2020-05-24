#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
        Null = -1,                       /*!< No Parameter*/
        ScreenSize,                      /*!< Screen Size Parameter*/
        WidgetPosition,                  /*!< Widget Position Parameter*/
        WidgetSize,                      /*!< Widget Size Parameter*/

        PlayMode,                        /*!< Play Mode Parameter*/
        Volume,                          /*!< Volume Parameter*/

        AutoPlay,                        /*!< Auto Play Parameter*/
        CurrentLanIndex,                 /*!< Current LanIndex Parameter*/
        CloseEvent,                      /*!< Close Event Parameter*/
        LastPlayIndex,                   /*!< Last Play Index Parameter*/
        CloseNetWork,                    /*!< Close NetWork Parameter*/
        FileAssociation,                 /*!< File Association Parameter*/
        ExtraDevicePath,                 /*!< Set Extra Device Path Parameter*/
        WindowConcise,                   /*!< Window Concise Parameter*/
        RemoteWidgetMode,                /*!< Remote Widget Mode Parameter*/
        WindowQuitMode,                  /*!< Window Quit Mode Parameter*/

        BackgroundTheme,                 /*!< Background Theme Parameter*/
        BackgroundTransparent,           /*!< Background Transparent Parameter*/
        BackgroundListTransparent,       /*!< Background List Transparent Parameter*/
        BackgroundTransparentEnable,     /*!< Background Transparent Enable Parameter*/

        OtherBackgroundLossless,         /*!< Other Background Lossless Parameter*/
        OtherCheckUpdate,                /*!< Other Check Update Parameter*/
        OtherSearch,                     /*!< Other Search Optimzied Parameter*/
        OtherUseAlbumCover,              /*!< Other Use File Album Parameter*/
        OtherUseInfo,                    /*!< Other Use File Info Parameter*/
        OtherWriteAlbumCover,            /*!< Other Write File Album Parameter*/
        OtherWriteInfo,                  /*!< Other Write File Info Parameter*/
        OtherSongFormat,                 /*!< Other Song Format Parameter*/
        OtherSideBy,                     /*!< Other App Side By Mode Parameter*/
        OtherSideByIn,                   /*!< Other App Side By In Mode Parameter*/
        OtherLrcKTVMode,                 /*!< Other Lrc KTV Mode Parameter*/
        OtherScreenSaverEnable,          /*!< Other Screen Saver Enable Parameter*/
        OtherScreenSaverTime,            /*!< Other Screen Saver Time Parameter*/
        OtherScreenSaverIndex,           /*!< Other Screen Saver Index Parameter*/

        RippleSpectrumEnable,            /*!< Ripple Spectrum Enable Parameter*/
        RippleSpectrumColor,             /*!< Ripple Spectrum Color Parameter*/

        ShowInteriorLrc,                 /*!< Show Interior Lrc Parameter*/
        ShowCortanaLrc,                  /*!< Show Cortana Lrc Parameter*/
        LrcColor,                        /*!< Lrc Color Parameter*/
        LrcSize,                         /*!< Lrc Size Parameter*/
        LrcType,                         /*!< Lrc Type Parameter*/
        LrcFamily,                       /*!< Lrc Family Parameter*/
        LrcFrontgroundColor,             /*!< Lrc Frontground Color Parameter*/
        LrcBackgroundColor,              /*!< Lrc Background Color Parameter*/
        LrcColorTrans,                   /*!< Lrc Color Trans Parameter*/

        ShowDesktopLrc,                  /*!< Show Desktop Lrc Parameter*/
        DLrcColor,                       /*!< Desktop Lrc Color Parameter*/
        DLrcSize,                        /*!< Desktop Lrc Size Parameter*/
        DLrcType,                        /*!< Desktop Lrc Type Parameter*/
        DLrcFamily,                      /*!< Desktop Lrc Family Parameter*/
        DLrcFrontgroundColor,            /*!< Desktop Lrc Frontground Color Parameter*/
        DLrcBackgroundColor,             /*!< Desktop Lrc Background Color Parameter*/
        DLrcColorTrans,                  /*!< Desktop Lrc Color Trans Parameter*/
        DLrcWindowType,                  /*!< Desktop Lrc Window Type Parameter*/
        DLrcSingleLineType,              /*!< Desktop Lrc Single Line Type Parameter*/
        DLrcLocked,                      /*!< Desktop Lrc Locked Parameter*/
        DLrcGeometry,                    /*!< Desktop Lrc Geometry Parameter*/

        EqualizerEnable,                 /*!< Equalizer Enable Parameter*/
        EqualizerValue,                  /*!< Equalizer Value Parameter*/
        EqualizerIndex,                  /*!< Equalizer Index Parameter*/
        EnhancedMusic,                   /*!< Enhanced Music Parameter*/
        EnhancedFadeEnable,              /*!< Enhanced Fade Enable Parameter*/
        EnhancedFadeInValue,             /*!< Enhanced Fade In Value Parameter*/
        EnhancedFadeOutValue,            /*!< Enhanced Fade Out Value Parameter*/
        EnhancedBS2B,                    /*!< Enhanced BS2B Parameter*/
        EnhancedCrossfade,               /*!< Enhanced Crossfade Parameter*/
        EnhancedStereo,                  /*!< Enhanced Stereo Parameter*/
        EnhancedLADSPA,                  /*!< Enhanced LADSPA Parameter*/
        EnhancedSOX,                     /*!< Enhanced SOX Parameter*/
        EnhancedSRC,                     /*!< Enhanced SRC Parameter*/
        EnhancedMonoStereo,              /*!< Enhanced Mono Stereo Parameter*/
        EnhancedMono,                    /*!< Enhanced Mono Parameter*/

        TimerAutoIndex,                  /*!< Timer Auto Index Parameter*/
        TimerAutoPlay,                   /*!< Timer Auto Play Parameter*/
        TimerAutoPlayHour,               /*!< Timer Auto Play Hour Parameter*/
        TimerAutoPlaySecond,             /*!< Timer Auto Play Second Parameter*/
        TimerAutoPlayRepeat,             /*!< Timer Auto Play Repeat Parameter*/
        TimerAutoPlayItemIndex,          /*!< Timer Auto Play Item Index Parameter*/
        TimerAutoPlaySongIndex,          /*!< Timer Auto Play Song Index Parameter*/
        TimerAutoStop,                   /*!< Timer Auto Stop Parameter*/
        TimerAutoStopHour,               /*!< Timer Auto Stop Hour Parameter*/
        TimerAutoStopSecond,             /*!< Timer Auto Stop Second Parameter*/
        TimerAutoStopRepeat,             /*!< Timer Auto Stop Repeat Parameter*/
        TimerAutoShutdown,               /*!< Timer Auto Shutdown Parameter*/
        TimerAutoShutdownHour,           /*!< Timer Auto Shutdown Hour Parameter*/
        TimerAutoShutdownSecond,         /*!< Timer Auto Shutdown Second Parameter*/
        TimerAutoShutdownRepeat,         /*!< Timer Auto Shutdown Repeat Parameter*/

        DownloadMusicPathDir,            /*!< Download Music Path Dir Parameter*/
        DownloadLrcPathDir,              /*!< Download Lrc Path Dir Parameter*/
        DownloadCacheLimit,              /*!< Download Cache Limit Parameter*/
        DownloadCacheSize,               /*!< Download Cache Size Parameter*/
        DownloadLimit,                   /*!< Download Limit Parameter*/
        DownloadServer,                  /*!< Download Server Parameter*/
        DownloadDLoadLimit,              /*!< Download DLoad Limit Parameter*/
        DownloadULoadLimit,              /*!< Download ULoad Limit Parameter*/

        HotkeyEnable,                    /*!< Hotkey Enable Parameter*/
        HotkeyString,                    /*!< Hotkey String Parameter*/

        NetworkCookie                    /*!< Network cookie Parameter*/

#ifdef MUSIC_MOBILE
        ,MobileWifiConnect               /*!< Mobile Wifi Connect Parameter*/
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
        return TTKStatic_cast(ConfigType, key);
    }

    QVariant m_variant;
    QMap<ConfigType, QVariant> m_para;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};

#define M_SETTING_PTR GetMusicSettingManager()
MUSIC_CORE_EXPORT MusicSettingManager* GetMusicSettingManager();

#endif // MUSICSETTINGMANAGER

