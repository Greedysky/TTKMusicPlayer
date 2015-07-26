#ifndef MUSICSINGLETON_H
#define MUSICSINGLETON_H

#include <QObject>
#include "musiclibexportglobal.h"

template <class T>
class MUSIC_CORE_EXPORT MusicSingleton
{
public:
    static inline T& createInstance()
    {
        static T obj;
        return obj;
    }
private:
    MusicSingleton(void){}
    ~MusicSingleton(void){}
    Q_DISABLE_COPY(MusicSingleton)
};

//Class that will implement the singleton mode,
//must use the macro in it's delare file
#define DECLARE_SINGLETON_CLASS( type ) \
    friend class MusicSingleton< type >;

#endif // MUSICSINGLETON

