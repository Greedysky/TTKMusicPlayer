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
    TTK_DECLARE_MODULE(MusicSettingManager)
public:
    enum Config
    {
        Null = -1,                                /*!< No Parameter*/
        ScreenSize = 0x1000,                      /*!< Screen Size Parameter*/
        WidgetPosition = 0x1001,                  /*!< Widget Position Parameter*/
        WidgetSize = 0x1002,                      /*!< Widget Size Parameter*/
        //
        ConfigVersion = 0x2000,                   /*!< Config Version Parameter*/
        PlayMode = 0x2001,                        /*!< Play Mode Parameter*/
        Volume = 0x2002,                          /*!< Volume Parameter*/
        LastPlayIndex = 0x2003,                   /*!< Last Play Index Parameter*/
        //
        LanguageIndex = 0x3000,                   /*!< Language Index Parameter*/
        StartUpMode = 0x3001,                     /*!< Start Up Mode Parameter*/
        StartUpPlayMode = 0x3002,                 /*!< Start Up Play Mode Parameter*/
        CloseEventMode = 0x3003,                  /*!< Close Event Parameter*/
        CloseNetWorkMode = 0x3004,                /*!< Close NetWork Parameter*/
        FileAssociationMode = 0x3005,             /*!< File Association Parameter*/
        FileAssociationValue = 0x3006,            /*!< File Association Value Parameter*/
        WindowConciseMode = 0x3007,               /*!< Window Concise Parameter*/
        RemoteWidgetMode = 0x3008,                /*!< Remote Widget Mode Parameter*/
        WindowQuitMode = 0x3009,                  /*!< Window Quit Mode Parameter*/
        ExtraDevicePath = 0x300A,                 /*!< Extra Device Path Parameter*/
        LastFileDialogPath = 0x300B,              /*!< Last File Dialog Path Parameter*/
        MediaLibraryPath = 0x300C,                /*!< Media Library Path Parameter*/
        //
        BackgroundThemeValue = 0x4000,            /*!< Background Theme Parameter*/
        BackgroundTransparent = 0x4001,           /*!< Background Transparent Parameter*/
        BackgroundListTransparent = 0x4002,       /*!< Background List Transparent Parameter*/
        BackgroundTransparentEnable = 0x4003,     /*!< Background Transparent Enable Parameter*/
        //
        HotkeyEnable = 0x5000,                    /*!< Hotkey Enable Parameter*/
        HotkeyValue = 0x5001,                     /*!< Hotkey String Parameter*/
        //
        OtherCheckUpdateEnable = 0x6000,          /*!< Other Check Update Parameter*/
        OtherReadAlbumCover = 0x6001,             /*!< Other Read File Album Parameter*/
        OtherReadFileInfo = 0x6002,               /*!< Other Read File Info Parameter*/
        OtherWriteAlbumCover = 0x6003,            /*!< Other Write File Album Parameter*/
        OtherWriteFileInfo = 0x6004,              /*!< Other Write File Info Parameter*/
        OtherSideByMode = 0x6005,                 /*!< Other App Side By Mode Parameter*/
        OtherSideByInMode = 0x6006,               /*!< Other App Side By In Mode Parameter*/
        OtherLrcKTVMode = 0x6007,                 /*!< Other Lrc KTV Mode Parameter*/
        OtherScreenSaverEnable = 0x6008,          /*!< Other Screen Saver Enable Parameter*/
        OtherScreenSaverTime = 0x6009,            /*!< Other Screen Saver Time Parameter*/
        OtherScreenSaverIndex = 0x600A,           /*!< Other Screen Saver Index Parameter*/
        OtherPlaylistAutoSaveEnable = 0x600B,     /*!< Other Playlist Auto Save Enable Parameter*/
        OtherRandomShuffleMode = 0x600C,          /*!< Other Random Shuffle Mode Parameter*/
        OtherHighDpiScalingEnable = 0x600D,       /*!< Other High DPI Scaling Enable Parameter*/
        OtherLogTrackEnable = 0x600E,             /*!< Other Log Track Enable Parameter*/
        //
        RippleLowPowerMode = 0x7000,              /*!< Ripple Low Power Mode Parameter*/
        RippleSpectrumEnable = 0x7001,            /*!< Ripple Spectrum Enable Parameter*/
        RippleSpectrumColor = 0x7002,             /*!< Ripple Spectrum Color Parameter*/
        RippleSpectrumTransparent = 0x7003,       /*!< Ripple Spectrum Transparent Parameter*/
        //
        ShowInteriorLrc = 0x8000,                 /*!< Show Interior Lrc Parameter*/
        LrcColor = 0x8001,                        /*!< Lrc Color Parameter*/
        LrcSize = 0x8002,                         /*!< Lrc Size Parameter*/
        LrcType = 0x8003,                         /*!< Lrc Type Parameter*/
        LrcFamily = 0x8004,                       /*!< Lrc Family Parameter*/
        LrcFrontgroundColor = 0x8005,             /*!< Lrc Frontground Color Parameter*/
        LrcBackgroundColor = 0x8006,              /*!< Lrc Background Color Parameter*/
        LrcColorTransparent = 0x8007,             /*!< Lrc Color Transparent Parameter*/
        //
        ShowDesktopLrc = 0x9000,                  /*!< Show Desktop Lrc Parameter*/
        DLrcColor = 0x9001,                       /*!< Desktop Lrc Color Parameter*/
        DLrcSize = 0x9002,                        /*!< Desktop Lrc Size Parameter*/
        DLrcType = 0x9003,                        /*!< Desktop Lrc Type Parameter*/
        DLrcFamily = 0x9004,                      /*!< Desktop Lrc Family Parameter*/
        DLrcFrontgroundColor = 0x9005,            /*!< Desktop Lrc Frontground Color Parameter*/
        DLrcBackgroundColor = 0x9006,             /*!< Desktop Lrc Background Color Parameter*/
        DLrcColorTransparent = 0x9007,            /*!< Desktop Lrc Color Transparent Parameter*/
        DLrcWindowMode = 0x9008,                  /*!< Desktop Lrc Window Mode Parameter*/
        DLrcSingleLineMode = 0x9009,              /*!< Desktop Lrc Single Line Mode Parameter*/
        DLrcLockedMode = 0x900A,                  /*!< Desktop Lrc Locked Mode Parameter*/
        DLrcGeometry = 0x900B,                    /*!< Desktop Lrc Geometry Parameter*/
        //
        EqualizerEnable = 0xA000,                 /*!< Equalizer Enable Parameter*/
        EqualizerValue = 0xA001,                  /*!< Equalizer Value Parameter*/
        EqualizerIndex = 0xA002,                  /*!< Equalizer Index Parameter*/
        EnhancedMusicIndex = 0xA003,              /*!< Enhanced Music Parameter*/
        EnhancedFadeEnable = 0xA004,              /*!< Enhanced Fade Enable Parameter*/
        EnhancedFadeInValue = 0xA005,             /*!< Enhanced Fade In Value Parameter*/
        EnhancedFadeOutValue = 0xA006,            /*!< Enhanced Fade Out Value Parameter*/
        EnhancedEffectValue = 0xA007,             /*!< Enhanced Effect Value Parameter*/
        //
        TimerAutoIndex = 0xB000,                  /*!< Timer Auto Index Parameter*/
        TimerAutoPlayMode = 0xB001,               /*!< Timer Auto Play Parameter*/
        TimerAutoPlayHour = 0xB002,               /*!< Timer Auto Play Hour Parameter*/
        TimerAutoPlaySecond = 0xB003,             /*!< Timer Auto Play Second Parameter*/
        TimerAutoPlayRepeat = 0xB004,             /*!< Timer Auto Play Repeat Parameter*/
        TimerAutoPlayItemIndex = 0xB005,          /*!< Timer Auto Play Item Index Parameter*/
        TimerAutoPlaySongIndex = 0xB006,          /*!< Timer Auto Play Song Index Parameter*/
        TimerAutoStopMode = 0xB007,               /*!< Timer Auto Stop Parameter*/
        TimerAutoStopHour = 0xB008,               /*!< Timer Auto Stop Hour Parameter*/
        TimerAutoStopSecond = 0xB009,             /*!< Timer Auto Stop Second Parameter*/
        TimerAutoStopRepeat = 0xB00A,             /*!< Timer Auto Stop Repeat Parameter*/
        TimerAutoShutdownMode = 0xB00B,           /*!< Timer Auto Shutdown Parameter*/
        TimerAutoShutdownHour = 0xB00C,           /*!< Timer Auto Shutdown Hour Parameter*/
        TimerAutoShutdownSecond = 0xB00D,         /*!< Timer Auto Shutdown Second Parameter*/
        TimerAutoShutdownRepeat = 0xB00E,         /*!< Timer Auto Shutdown Repeat Parameter*/
        //
        DownloadMusicDirPath = 0xC000,            /*!< Download Music Dir Path Parameter*/
        DownloadLrcDirPath = 0xC001,              /*!< Download Lrc Dir Path Parameter*/
        DownloadServerIndex = 0xC002,             /*!< Download Server Parameter*/
        DownloadCacheEnable = 0xC003,             /*!< Download Cache Enable Parameter*/
        DownloadCacheSize = 0xC004,               /*!< Download Cache Size Parameter*/
        DownloadLimitEnable = 0xC005,             /*!< Download Limit Parameter*/
        DownloadDownloadLimitSize = 0xC006,       /*!< Download Download Limit Parameter*/
        DownloadUploadLimitSize = 0xC007,         /*!< Download Upload Limit Parameter*/
        DownloadFileNameRule = 0xC008,            /*!< Download File Name Rule Parameter*/
    };

    /*!
     * Set current value by Config Type.
     */
    inline void setValue(Config type, const QVariant &var)
    {
        m_parameter[type] = var;
    }

    /*!
     * Set current value by String Type.
     */
    inline void setValue(const QString &stype, const QVariant &var)
    {
        m_parameter[stringToEnum(stype)] = var;
    }

    /*!
     * Get current value by Config Type.
     */
    inline QVariant value(Config type) const
    {
        return m_parameter[type];
    }

    /*!
     * Get current value by String Type.
     */
    inline QVariant value(const QString &stype) const
    {
        return m_parameter[stringToEnum(stype)];
    }

    /*!
     * Get parameter count.
     */
    inline int count() const
    {
        return m_parameter.count();
    }

    /*!
     * Current parameter is empty.
     */
    inline bool isEmpty() const
    {
        return m_parameter.isEmpty();
    }

    /*!
     * Current parameter contains type.
     */
    inline bool contains(Config type) const
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

    TTK_DECLARE_SINGLETON_CLASS(MusicSettingManager)

};

#define G_SETTING_PTR makeMusicSettingManager()
TTK_MODULE_EXPORT MusicSettingManager* makeMusicSettingManager();

#endif // MUSICSETTINGMANAGER

