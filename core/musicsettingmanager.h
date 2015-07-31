#ifndef MUSICSETTINGMANAGER
#define MUSICSETTINGMANAGER

#include "musicsingletone.h"
#include "musicobject.h"

#define M_SETTING MusicSingleton<MusicSettingManager>::createInstance()

class MUSIC_CORE_EXPORT MusicSettingManager
{

public:
    ~MusicSettingManager(){}
    inline void setValue(MusicObject::XMLConfigType type,
                         const QVariant& var)
    {
        m_para[type] = var;
    }

    inline QVariant value(MusicObject::XMLConfigType type) const
    {
        return m_para[type];
    }

    inline int count() const
    {
        return m_para.count();
    }

    inline bool isEmpty() const
    {
        return m_para.isEmpty();
    }

protected:
    MusicSettingManager(){}
    QMap<MusicObject::XMLConfigType, QVariant> m_para;

    DECLARE_SINGLETON_CLASS(MusicSettingManager)
};


#endif // MUSICSETTINGMANAGER

