#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
    Q_ENUMS(ConfigType)
    TTK_DECLARE_MODULE(MusicSettingManager)
public:
    enum ConfigType
    {
        Null = -1,                       /*!< No Parameter*/
        ScreenSize,                      /*!< Screen Size Parameter*/
        WidgetPosition,                  /*!< Widget Position Parameter*/
        WidgetSize,                      /*!< Widget Size Parameter*/
        //
        ConfigVersion,                   /*!< Config Version Parameter*/
        PlayMode,                        /*!< Play Mode Parameter*/
        Volume,                          /*!< Volume Parameter*/
        //
        AutoPlayMode,                    /*!< Auto Play Parameter*/
        CurrentLanIndex,                 /*!< Current LanIndex Parameter*/
        CloseEventMode,                  /*!< Close Event Parameter*/
        LastPlayIndex,                   /*!< Last Play Index Parameter*/
        CloseNetWorkMode,                /*!< Close NetWork Parameter*/
        FileAssociationMode,             /*!< File Association Parameter*/
        ExtraDevicePath,                 /*!< Set Extra Device Path Parameter*/
        WindowConciseMode,               /*!< Window Concise Parameter*/
        RemoteWidgetMode,                /*!< Remote Widget Mode Parameter*/
        WindowQuitMode,                  /*!< Window Quit Mode Parameter*/
        //
        BackgroundThemeValue,            /*!< Background Theme Parameter*/
        BackgroundTransparent,           /*!< Background Transparent Parameter*/
        BackgroundListTransparent,       /*!< Background List Transparent Parameter*/
        BackgroundTransparentEnable,     /*!< Background Transparent Enable Parameter*/
        //
        OtherCheckUpdateEnable,          /*!< Other Check Update Parameter*/
        OtherUseAlbumCover,              /*!< Other Use File Album Parameter*/
        OtherUseFileInfo,                /*!< Other Use File Info Parameter*/
        OtherWriteAlbumCover,            /*!< Other Write File Album Parameter*/
        OtherWriteFileInfo,              /*!< Other Write File Info Parameter*/
        OtherSideByMode,                 /*!< Other App Side By Mode Parameter*/
        OtherSideByInMode,               /*!< Other App Side By In Mode Parameter*/
        OtherLrcKTVMode,                 /*!< Other Lrc KTV Mode Parameter*/
        OtherScreenSaverEnable,          /*!< Other Screen Saver Enable Parameter*/
        OtherScreenSaverTime,            /*!< Other Screen Saver Time Parameter*/
        OtherScreenSaverIndex,           /*!< Other Screen Saver Index Parameter*/
        //
        RippleLowPowerMode,              /*!< Ripple Low Power Mode Parameter*/
        RippleSpectrumEnable,            /*!< Ripple Spectrum Enable Parameter*/
        RippleSpectrumColor,             /*!< Ripple Spectrum Color Parameter*/
        //
        ShowInteriorLrc,                 /*!< Show Interior Lrc Parameter*/
        LrcColor,                        /*!< Lrc Color Parameter*/
        LrcSize,                         /*!< Lrc Size Parameter*/
        LrcType,                         /*!< Lrc Type Parameter*/
        LrcFamily,                       /*!< Lrc Family Parameter*/
        LrcFrontgroundColor,             /*!< Lrc Frontground Color Parameter*/
        LrcBackgroundColor,              /*!< Lrc Background Color Parameter*/
        LrcColorTransparent,             /*!< Lrc Color Transparent Parameter*/
        //
        ShowDesktopLrc,                  /*!< Show Desktop Lrc Parameter*/
        DLrcColor,                       /*!< Desktop Lrc Color Parameter*/
        DLrcSize,                        /*!< Desktop Lrc Size Parameter*/
        DLrcType,                        /*!< Desktop Lrc Type Parameter*/
        DLrcFamily,                      /*!< Desktop Lrc Family Parameter*/
        DLrcFrontgroundColor,            /*!< Desktop Lrc Frontground Color Parameter*/
        DLrcBackgroundColor,             /*!< Desktop Lrc Background Color Parameter*/
        DLrcColorTransparent,            /*!< Desktop Lrc Color Transparent Parameter*/
        DLrcWindowMode,                  /*!< Desktop Lrc Window Mode Parameter*/
        DLrcSingleLineMode,              /*!< Desktop Lrc Single Line Mode Parameter*/
        DLrcLockedMode,                  /*!< Desktop Lrc Locked Mode Parameter*/
        DLrcGeometry,                    /*!< Desktop Lrc Geometry Parameter*/
        //
        EqualizerEnable,                 /*!< Equalizer Enable Parameter*/
        EqualizerValue,                  /*!< Equalizer Value Parameter*/
        EqualizerIndex,                  /*!< Equalizer Index Parameter*/
        EnhancedMusicIndex,              /*!< Enhanced Music Parameter*/
        EnhancedFadeEnable,              /*!< Enhanced Fade Enable Parameter*/
        EnhancedFadeInValue,             /*!< Enhanced Fade In Value Parameter*/
        EnhancedFadeOutValue,            /*!< Enhanced Fade Out Value Parameter*/
        EnhancedEffectValue,             /*!< Enhanced Effect Value Parameter*/
        //
        TimerAutoIndex,                  /*!< Timer Auto Index Parameter*/
        TimerAutoPlayMode,               /*!< Timer Auto Play Parameter*/
        TimerAutoPlayHour,               /*!< Timer Auto Play Hour Parameter*/
        TimerAutoPlaySecond,             /*!< Timer Auto Play Second Parameter*/
        TimerAutoPlayRepeat,             /*!< Timer Auto Play Repeat Parameter*/
        TimerAutoPlayItemIndex,          /*!< Timer Auto Play Item Index Parameter*/
        TimerAutoPlaySongIndex,          /*!< Timer Auto Play Song Index Parameter*/
        TimerAutoStopMode,               /*!< Timer Auto Stop Parameter*/
        TimerAutoStopHour,               /*!< Timer Auto Stop Hour Parameter*/
        TimerAutoStopSecond,             /*!< Timer Auto Stop Second Parameter*/
        TimerAutoStopRepeat,             /*!< Timer Auto Stop Repeat Parameter*/
        TimerAutoShutdownMode,           /*!< Timer Auto Shutdown Parameter*/
        TimerAutoShutdownHour,           /*!< Timer Auto Shutdown Hour Parameter*/
        TimerAutoShutdownSecond,         /*!< Timer Auto Shutdown Second Parameter*/
        TimerAutoShutdownRepeat,         /*!< Timer Auto Shutdown Repeat Parameter*/
        //
        DownloadMusicDirPath,            /*!< Download Music Dir Path Parameter*/
        DownloadLrcDirPath,              /*!< Download Lrc Dir Path Parameter*/
        DownloadServerIndex,             /*!< Download Server Parameter*/
        DownloadCacheEnable,             /*!< Download Cache Enable Parameter*/
        DownloadCacheSize,               /*!< Download Cache Size Parameter*/
        DownloadLimitEnable,             /*!< Download Limit Parameter*/
        DownloadDownloadLimitSize,       /*!< Download Download Limit Parameter*/
        DownloadUploadLimitSize,         /*!< Download Upload Limit Parameter*/
        //
        HotkeyEnable,                    /*!< Hotkey Enable Parameter*/
        HotkeyValue                      /*!< Hotkey String Parameter*/
    };

    /*!
     * Set current value by Config Type.
     */
    inline void setValue(ConfigType type, const QVariant &var)
    {
        m_parameter[type] = var;
    }

    /*!
     * Set current value by String Type.
     */
    inline void setValue(const QString &stype, const QVariant &var)
    {
        m_parameter[typeStringToEnum(stype)] = var;
    }

    /*!
     * Get current value by Config Type.
     */
    inline QVariant value(ConfigType type) const
    {
        return m_parameter[type];
    }

    /*!
     * Get current value by String Type.
     */
    inline QVariant value(const QString &stype) const
    {
        return m_parameter[typeStringToEnum(stype)];
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
    inline bool contains(ConfigType type) const
    {
        return m_parameter.contains(type);
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
    QMap<ConfigType, QVariant> m_parameter;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};

#define G_SETTING_PTR makeMusicSettingManager()
TTK_MODULE_EXPORT MusicSettingManager* makeMusicSettingManager();

#endif // MUSICSETTINGMANAGER

