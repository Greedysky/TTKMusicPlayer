#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

#include "musicsingletone.h"

#define M_SETTING (MusicSingleton<MusicSettingManager>::createInstance())

class MUSIC_CORE_EXPORT MusicSettingManager
{
public:
    enum ConfigType {
        PlayModeChoiced,
        VolumeChoiced,
        AutoPlayChoiced,
        Play3DMusicChoiced,
        CurrentLanIndexChoiced,
        CloseEventChoiced,
        LastPlayIndexChoiced,
        BgThemeChoiced,
        BgTransparentChoiced,

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
        DLrcBgColorchoiced,
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
        TimerAutoShutdownRepeatChoiced

    };

    ~MusicSettingManager(){}
    inline void setValue(ConfigType type, const QVariant& var)
    {
        m_para[type] = var;
    }

    inline void setValue(const QString& stype, const QVariant& var)
    {
        m_para[typeStringToEnum(stype)] = var;
    }

    inline QVariant value(ConfigType type) const
    {
        return m_para[type];
    }

    inline QVariant value(const QString& stype)
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
    ConfigType typeStringToEnum(const QString& stype)
    {
        ConfigType type;
        if(stype == "PlayModeChoiced") type = PlayModeChoiced;
        if(stype == "VolumeChoiced") type = VolumeChoiced;
        if(stype == "AutoPlayChoiced") type = AutoPlayChoiced;
        if(stype == "Play3DMusicChoiced") type = Play3DMusicChoiced;
        if(stype == "CurrentLanIndexChoiced") type = CurrentLanIndexChoiced;
        if(stype == "CloseEventChoiced") type = CloseEventChoiced;
        if(stype == "LastPlayIndexChoiced") type = LastPlayIndexChoiced;
        if(stype == "BgThemeChoiced") type = BgThemeChoiced;
        if(stype == "BgTransparentChoiced") type = BgTransparentChoiced;

        if(stype == "ShowInlineLrcChoiced") type = ShowInlineLrcChoiced;
        if(stype == "ShowDesktopLrcChoiced") type = ShowDesktopLrcChoiced;
        if(stype == "LrcColorChoiced") type = LrcColorChoiced;
        if(stype == "LrcSizeChoiced") type = LrcSizeChoiced;
        if(stype == "LrcTypeChoiced") type = LrcTypeChoiced;
        if(stype == "LrcFamilyChoiced") type = LrcFamilyChoiced;
        if(stype == "LrcFgColorChoiced") type = LrcFgColorChoiced;
        if(stype == "LrcBgColorChoiced") type = LrcBgColorChoiced;
        if(stype == "LrcColorTransChoiced") type = LrcColorTransChoiced;
        if(stype == "DLrcColorChoiced") type = DLrcColorChoiced;
        if(stype == "DLrcSizeChoiced") type = DLrcSizeChoiced;
        if(stype == "DLrcFamilyChoiced") type = DLrcFamilyChoiced;
        if(stype == "DLrcFgColorChoiced") type = DLrcFgColorChoiced;
        if(stype == "DLrcBgColorchoiced") type = DLrcBgColorchoiced;
        if(stype == "DLrcColorTransChoiced") type = DLrcColorTransChoiced;
        if(stype == "DLrcLockedChoiced") type = DLrcLockedChoiced;
        if(stype == "DLrcGeometryChoiced") type = DLrcGeometryChoiced;

        if(stype == "EqualizerEnableChoiced") type = EqualizerEnableChoiced;
        if(stype == "EqualizerValueChoiced") type = EqualizerValueChoiced;
        if(stype == "EqualizerIndexChoiced") type = EqualizerIndexChoiced;
        if(stype == "SequalizerIndexChoiced") type = SequalizerIndexChoiced;

        if(stype == "TimerAutoIndexChoiced") type = TimerAutoIndexChoiced;
        if(stype == "TimerAutoPlayChoiced") type = TimerAutoPlayChoiced;
        if(stype == "TimerAutoPlayHourChoiced") type = TimerAutoPlayHourChoiced;
        if(stype == "TimerAutoPlaySecondChoiced") type = TimerAutoPlaySecondChoiced;
        if(stype == "TimerAutoPlayRepeatChoiced") type = TimerAutoPlayRepeatChoiced;
        if(stype == "TimerAutoPlayItemIndexChoiced") type = TimerAutoPlayItemIndexChoiced;
        if(stype == "TimerAutoPlaySongIndexChoiced") type = TimerAutoPlaySongIndexChoiced;
        if(stype == "TimerAutoStopChoiced") type = TimerAutoStopChoiced;
        if(stype == "TimerAutoStopHourChoiced") type = TimerAutoStopHourChoiced;
        if(stype == "TimerAutoStopSecondChoiced") type = TimerAutoStopSecondChoiced;
        if(stype == "TimerAutoStopRepeatChoiced") type = TimerAutoStopRepeatChoiced;
        if(stype == "TimerAutoShutdownChoiced") type = TimerAutoShutdownChoiced;
        if(stype == "TimerAutoShutdownHourChoiced") type = TimerAutoShutdownHourChoiced;
        if(stype == "TimerAutoShutdownSecondChoiced") type = TimerAutoShutdownSecondChoiced;
        if(stype == "TimerAutoShutdownRepeatChoiced") type = TimerAutoShutdownRepeatChoiced;

        return type;
    }

    QVariant m_variant;
    QMap<ConfigType, QVariant> m_para;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};


#endif // MUSICSETTINGMANAGER

