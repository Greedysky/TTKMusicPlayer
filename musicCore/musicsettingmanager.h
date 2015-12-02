#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"
#include <QMetaEnum>

#define M_SETTING (MusicSingleton<MusicSettingManager>::createInstance())

class MUSIC_CORE_EXPORT MusicSettingManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConfigType)
public:
    enum ConfigType
    {
        Null = -1,
        ScreenSize,

        PlayModeChoiced,
        VolumeChoiced,

        AutoPlayChoiced,
        EnhancedMusicChoiced,
        CurrentLanIndexChoiced,
        CloseEventChoiced,
        LastPlayIndexChoiced,
        CloseNetWorkChoiced,
        FileAssociationChoiced,

        BgThemeChoiced,
        BgTransparentChoiced,
        BgListTransparentChoiced,

        ShowInlineLrcChoiced,
        ShowDesktopLrcChoiced,
        LrcColorChoiced,
        LrcSizeChoiced,
        LrcTypeChoiced,
        LrcFamilyChoiced,
        LrcFgColorChoiced,
        LrcBgColorChoiced,
        LrcColorTransChoiced,

        DLrcColorChoiced,
        DLrcSizeChoiced,
        DLrcTypeChoiced,
        DLrcFamilyChoiced,
        DLrcFgColorChoiced,
        DLrcBgColorChoiced,
        DLrcColorTransChoiced,
        DLrcLockedChoiced,
        DLrcGeometryChoiced,

        EqualizerEnableChoiced,
        EqualizerValueChoiced,
        EqualizerIndexChoiced,
        SequalizerIndexChoiced,

        TimerAutoIndexChoiced,
        TimerAutoPlayChoiced,
        TimerAutoPlayHourChoiced,
        TimerAutoPlaySecondChoiced,
        TimerAutoPlayRepeatChoiced,
        TimerAutoPlayItemIndexChoiced,
        TimerAutoPlaySongIndexChoiced,
        TimerAutoStopChoiced,
        TimerAutoStopHourChoiced,
        TimerAutoStopSecondChoiced,
        TimerAutoStopRepeatChoiced,
        TimerAutoShutdownChoiced,
        TimerAutoShutdownHourChoiced,
        TimerAutoShutdownSecondChoiced,
        TimerAutoShutdownRepeatChoiced,

        DownloadMusicPathDirChoiced,
        DownloadLrcPathDirChoiced,
        DownloadCacheLimitChoiced,
        DownloadCacheSizeChoiced,
        DownloadLimitChoiced,
        DownloadDLoadLimitChoiced,
        DownloadULoadLimitChoiced

    };

    inline void setValue(ConfigType type, const QVariant &var)
    {
        m_para[type] = var;
    }

    inline void setValue(const QString &stype, const QVariant &var)
    {
        m_para[typeStringToEnum(stype)] = var;
    }

    inline QVariant value(ConfigType type) const
    {
        return m_para[type];
    }

    inline QVariant value(const QString &stype) const
    {
        return m_para[typeStringToEnum(stype)];
    }

    inline int count() const
    {
        return m_para.count();
    }

    inline bool isEmpty() const
    {
        return m_para.isEmpty();
    }

    inline bool contains(ConfigType type) const
    {
        return m_para.contains(type);
    }

protected:
    MusicSettingManager(){}
    ~MusicSettingManager(){}

    ConfigType typeStringToEnum(const QString &stype) const
    {
        int index = staticMetaObject.indexOfEnumerator("ConfigType");
        QMetaEnum metaEnum = staticMetaObject.enumerator(index);
        int key = metaEnum.keyToValue(stype.toStdString().c_str());
        return static_cast<ConfigType>(key);
    }

    QVariant m_variant;
    QMap<ConfigType, QVariant> m_para;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};


#endif // MUSICSETTINGMANAGER

