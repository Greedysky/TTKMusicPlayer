#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include <QMetaEnum>
#include "musicobject.h"
#include "ttksingleton.h"

/*! @brief The class of the paramater setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSettingManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(Config)
    TTK_DECLARE_SINGLETON_CLASS(MusicSettingManager)
public:
    enum Config
    {
        Null = -1,                                /*!< No parameter */
        ScreenSize = 0x1000,                      /*!< Screen size parameter */
        WidgetPosition = 0x1001,                  /*!< Widget position parameter */
        WidgetSize = 0x1002,                      /*!< Widget size parameter */
        //
        ConfigVersion = 0x2000,                   /*!< Config version parameter */
        PlayMode = 0x2001,                        /*!< Play mode parameter */
        Volume = 0x2002,                          /*!< Volume parameter */
        LastPlayIndex = 0x2003,                   /*!< Last play index parameter */
        UserPermission = 0x2004,                  /*!< User permission parameter */
        //
        LanguageIndex = 0x3000,                   /*!< Language index parameter */
        StartUpMode = 0x3001,                     /*!< Start up mode parameter */
        StartUpPlayMode = 0x3002,                 /*!< Start up play mode parameter */
        CloseEventMode = 0x3003,                  /*!< Close event parameter */
        CloseNetWorkMode = 0x3004,                /*!< Close netWork parameter */
        FileAssociationMode = 0x3005,             /*!< File association parameter */
        FileAssociationValue = 0x3006,            /*!< File association value parameter */
        WindowConciseMode = 0x3007,               /*!< Window concise parameter */
        RemoteWidgetMode = 0x3008,                /*!< Remote widget mode parameter */
        WindowQuitMode = 0x3009,                  /*!< Window quit mode parameter */
        ExtraDevicePath = 0x300A,                 /*!< Extra device path parameter */
        LastFileDialogPath = 0x300B,              /*!< Last file dialog path parameter */
        MediaLibraryPath = 0x300C,                /*!< Media library path parameter */
        //
        BackgroundThemeValue = 0x4000,            /*!< Background theme parameter */
        BackgroundTransparent = 0x4001,           /*!< Background transparent parameter */
        BackgroundListTransparent = 0x4002,       /*!< Background list transparent parameter */
        BackgroundTransparentEnable = 0x4003,     /*!< Background transparent enable parameter */
        //
        HotkeyEnable = 0x5000,                    /*!< Hotkey enable parameter */
        HotkeyValue = 0x5001,                     /*!< Hotkey string parameter */
        //
        OtherCheckUpdateEnable = 0x6000,          /*!< Other check update parameter */
        OtherReadAlbumCover = 0x6001,             /*!< Other read file album parameter */
        OtherReadFileInfo = 0x6002,               /*!< Other read file info parameter */
        OtherWriteAlbumCover = 0x6003,            /*!< Other write file album parameter */
        OtherWriteFileInfo = 0x6004,              /*!< Other write file info parameter */
        OtherSideByMode = 0x6005,                 /*!< Other app side by mode parameter */
        OtherSideByInMode = 0x6006,               /*!< Other app side by in mode parameter */
        OtherLrcKTVMode = 0x6007,                 /*!< Other lrc KTV mode parameter */
        OtherScreenSaverEnable = 0x6008,          /*!< Other screen saver enable parameter */
        OtherScreenSaverTime = 0x6009,            /*!< Other screen saver time parameter */
        OtherScreenSaverIndex = 0x600A,           /*!< Other screen saver index parameter */
        OtherPlaylistAutoSaveEnable = 0x600B,     /*!< Other playlist auto save enable parameter */
        OtherRandomShuffleMode = 0x600C,          /*!< Other random shuffle mode parameter */
        OtherHighDpiScalingEnable = 0x600D,       /*!< Other high DPI scaling enable parameter */
        OtherLogTrackEnable = 0x600E,             /*!< Other log track enable parameter */
        //
        RippleLowPowerMode = 0x7000,              /*!< Ripple low power mode parameter */
        RippleSpectrumEnable = 0x7001,            /*!< Ripple spectrum enable parameter */
        RippleSpectrumColor = 0x7002,             /*!< Ripple spectrum color parameter */
        RippleSpectrumTransparent = 0x7003,       /*!< Ripple spectrum transparent parameter */
        //
        ShowInteriorLrc = 0x8000,                 /*!< Show interior lrc parameter */
        LrcColor = 0x8001,                        /*!< Lrc color parameter */
        LrcSize = 0x8002,                         /*!< Lrc size parameter */
        LrcType = 0x8003,                         /*!< Lrc type parameter */
        LrcFamily = 0x8004,                       /*!< Lrc family parameter */
        LrcFrontgroundColor = 0x8005,             /*!< Lrc frontground color parameter */
        LrcBackgroundColor = 0x8006,              /*!< Lrc background color parameter */
        LrcColorTransparent = 0x8007,             /*!< Lrc color transparent parameter */
        //
        ShowDesktopLrc = 0x9000,                  /*!< Show desktop lrc parameter */
        DLrcColor = 0x9001,                       /*!< Desktop lrc color parameter */
        DLrcSize = 0x9002,                        /*!< Desktop lrc size parameter */
        DLrcType = 0x9003,                        /*!< Desktop lrc type parameter */
        DLrcFamily = 0x9004,                      /*!< Desktop lrc family parameter */
        DLrcFrontgroundColor = 0x9005,            /*!< Desktop lrc frontground color parameter */
        DLrcBackgroundColor = 0x9006,             /*!< Desktop lrc background color parameter */
        DLrcColorTransparent = 0x9007,            /*!< Desktop lrc color transparent parameter */
        DLrcWindowMode = 0x9008,                  /*!< Desktop lrc window mode parameter */
        DLrcSingleLineMode = 0x9009,              /*!< Desktop lrc single line Mode parameter */
        DLrcLockedMode = 0x900A,                  /*!< Desktop lrc locked mode parameter */
        DLrcGeometry = 0x900B,                    /*!< Desktop lrc geometry parameter */
        //
        EqualizerEnable = 0xA000,                 /*!< Equalizer enable parameter */
        EqualizerValue = 0xA001,                  /*!< Equalizer value parameter */
        EqualizerIndex = 0xA002,                  /*!< Equalizer index parameter */
        EnhancedMusicIndex = 0xA003,              /*!< Enhanced music parameter */
        EnhancedFadeEnable = 0xA004,              /*!< Enhanced fade enable parameter */
        EnhancedFadeInValue = 0xA005,             /*!< Enhanced fade in value parameter */
        EnhancedFadeOutValue = 0xA006,            /*!< Enhanced fade out value parameter */
        EnhancedEffectValue = 0xA007,             /*!< Enhanced effect value parameter */
        //
        TimerAutoIndex = 0xB000,                  /*!< Timer auto index parameter */
        TimerAutoPlayMode = 0xB001,               /*!< Timer auto play parameter */
        TimerAutoPlayHour = 0xB002,               /*!< Timer auto play hour parameter */
        TimerAutoPlaySecond = 0xB003,             /*!< Timer auto play second parameter */
        TimerAutoPlayRepeat = 0xB004,             /*!< Timer auto play repeat parameter */
        TimerAutoPlayItemIndex = 0xB005,          /*!< Timer auto play item index parameter */
        TimerAutoPlaySongIndex = 0xB006,          /*!< Timer auto play song index parameter */
        TimerAutoStopMode = 0xB007,               /*!< Timer auto stop parameter */
        TimerAutoStopHour = 0xB008,               /*!< Timer auto stop hour parameter */
        TimerAutoStopSecond = 0xB009,             /*!< Timer auto stop second parameter */
        TimerAutoStopRepeat = 0xB00A,             /*!< Timer auto stop repeat parameter */
        TimerAutoShutdownMode = 0xB00B,           /*!< Timer auto shutdown parameter */
        TimerAutoShutdownHour = 0xB00C,           /*!< Timer auto shutdown Hour parameter */
        TimerAutoShutdownSecond = 0xB00D,         /*!< Timer auto shutdown Second parameter */
        TimerAutoShutdownRepeat = 0xB00E,         /*!< Timer auto shutdown Repeat parameter */
        //
        DownloadMusicDirPath = 0xC000,            /*!< Download music dir path parameter */
        DownloadLrcDirPath = 0xC001,              /*!< Download lrc dir path parameter */
        DownloadServerIndex = 0xC002,             /*!< Download server parameter */
        DownloadCacheEnable = 0xC003,             /*!< Download cache enable parameter */
        DownloadCacheSize = 0xC004,               /*!< Download cache size parameter */
        DownloadLimitEnable = 0xC005,             /*!< Download limit parameter */
        DownloadDownloadLimitSize = 0xC006,       /*!< Download download limit parameter */
        DownloadUploadLimitSize = 0xC007,         /*!< Download upload limit parameter */
        DownloadFileNameRule = 0xC008,            /*!< Download file name rule parameter */
        DownloadServerPriority = 0xC009,          /*!< Download server priority parameter */
    };

    /*!
     * Set current value by Config Type.
     */
    inline void setValue(Config type, const QVariant &var) noexcept
    {
        m_parameter[type] = var;
    }

    /*!
     * Set current value by String Type.
     */
    inline void setValue(const QString &stype, const QVariant &var) noexcept
    {
        m_parameter[stringToEnum(stype)] = var;
    }

    /*!
     * Get current value by Config Type.
     */
    inline QVariant value(Config type) const noexcept
    {
        return m_parameter[type];
    }

    /*!
     * Get current value by String Type.
     */
    inline QVariant value(const QString &stype) const noexcept
    {
        return m_parameter[stringToEnum(stype)];
    }

    /*!
     * Get parameter count.
     */
    inline int count() const noexcept
    {
        return m_parameter.count();
    }

    /*!
     * Current parameter is empty.
     */
    inline bool isEmpty() const noexcept
    {
        return m_parameter.isEmpty();
    }

    /*!
     * Current parameter contains type.
     */
    inline bool contains(Config type) const noexcept
    {
        return m_parameter.contains(type);
    }

private:
    /*!
     * Convert String type to Config Type.
     */
    inline Config stringToEnum(const QString &stype) const
    {
        const int index = staticMetaObject.indexOfEnumerator("Config");
        const QMetaEnum &metaEnum = staticMetaObject.enumerator(index);
        const int key = metaEnum.keyToValue(stype.toStdString().c_str());
        return TTKStaticCast(Config, key);
    }

    QVariant m_variant;
    QMap<Config, QVariant> m_parameter;

};

#define G_SETTING_PTR makeMusicSettingManager()
TTK_MODULE_EXPORT MusicSettingManager* makeMusicSettingManager();

#endif // MUSICSETTINGMANAGER

