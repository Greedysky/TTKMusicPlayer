#ifndef MUSICSINGLETON_H
#define MUSICSINGLETON_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMutex>
#include <QScopedPointer>
#include "musicglobaldefine.h"

template <typename T>
/*! @brief The class of the singleton base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSingleton
{
public:
    static T* createInstance();
    /*!
     * Get object instance ptr.
     */
    static QString getClassName();
    /*!
     * Get class object name.
     */

private:
    MusicSingleton(){}   
    /*!
     * Object contsructor.
     */
    ~MusicSingleton(){}

    static QMutex m_mutex;
    static QScopedPointer<T> m_instance;
    Q_DISABLE_COPY(MusicSingleton)

};

    ////////////////////////////////////////////////////////////////
    ///                                                           //
    ///              Singleton Implementation                     //
    ///                                                           //
    ////////////////////////////////////////////////////////////////

template<typename T> QMutex MusicSingleton<T>::m_mutex;
template<typename T> QScopedPointer<T> MusicSingleton<T>::m_instance;

template<typename T>
T* MusicSingleton<T>::createInstance()
{
    if(m_instance.isNull())
    {
        m_mutex.lock();
        if(m_instance.isNull())
        {
            m_instance.reset(new T());
        }
        m_mutex.unlock();
    }
    return m_instance.data();
}

template<typename T>
QString MusicSingleton<T>::getClassName()
{
    return "MusicSingleton";
}

    ////////////////////////////////////////////////////////////////
    ///                                                           //
    ///              Singleton Macro                              //
    ///                                                           //
    ////////////////////////////////////////////////////////////////

////Class that will implement the singleton mode,
////must use the macro in it's delare file
#define DECLARE_SINGLETON_CLASS( Class )                        \
    friend class MusicSingleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;

#endif // MUSICSINGLETON

