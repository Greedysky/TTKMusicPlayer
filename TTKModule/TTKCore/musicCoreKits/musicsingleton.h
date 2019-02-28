#ifndef MUSICSINGLETON_H
#define MUSICSINGLETON_H

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

#include <QMutex>
#include <QObject>
#include <QScopedPointer>
#include "musicglobaldefine.h"

template <typename T>
/*! @brief The class of the singleton base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSingleton
{
    TTK_DECLARE_MODULE(MusicSingleton)
public:
    /*!
     * Get object instance ptr.
     */
    static T* createInstance();

private:
    /*!
     * Object contsructor.
     */
    MusicSingleton(){}   

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

template <typename T> QMutex MusicSingleton<T>::m_mutex;
template <typename T> QScopedPointer<T> MusicSingleton<T>::m_instance;

template <typename T>
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

    ////////////////////////////////////////////////////////////////
    ///                                                           //
    ///              Singleton Macro                              //
    ///                                                           //
    ////////////////////////////////////////////////////////////////

////Class that will implement the singleton mode,
////must use the macro in it's delare file
#define DECLARE_SINGLETON_CLASS( Class )             \
    friend class MusicSingleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>;

#endif // MUSICSINGLETON

