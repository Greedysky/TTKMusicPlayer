#ifndef MUSICSINGLEMANAGER_H
#define MUSICSINGLEMANAGER_H

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

#include <QMap>
#include "musicsingleton.h"

#define M_SINGLE_MANAGER_PTR (MusicSingleton<MusicSingleManager>::createInstance())
//
#define M_SINGLE_MANAGER_WIDGET_NEW2(name, parent)                      \
    MusicSingleManager *manager = M_SINGLE_MANAGER_PTR;                 \
    if(!manager->contains(#name))                                       \
    {                                                                   \
        manager->createObject(#name, new name(parent));                 \
    }                                                                   \
    name *w = MStatic_cast(name*, manager->object(#name));

#define M_SINGLE_MANAGER_WIDGET_NEW(name)                               \
    M_SINGLE_MANAGER_WIDGET_NEW2(name, MusicApplication::instance())

#define M_SINGLE_MANAGER_WIDGET_CLASS2(name, parent)                    \
{                                                                       \
    M_SINGLE_MANAGER_WIDGET_NEW2(name, parent)                          \
    w->raise();                                                         \
    w->show();                                                          \
}

#define M_SINGLE_MANAGER_WIDGET_CLASS(name)                             \
    M_SINGLE_MANAGER_WIDGET_CLASS2(name, MusicApplication::instance())
//
#define M_SINGLE_MANAGER_DIALOG_NEW2(name, parent)                      \
    MusicSingleManager *manager = M_SINGLE_MANAGER_PTR;                 \
    if(!manager->contains(#name))                                       \
    {                                                                   \
        manager->createObject(#name, new name(parent));                 \
    }                                                                   \
    name *w = MStatic_cast(name*, manager->object(#name));

#define M_SINGLE_MANAGER_DIALOG_NEW(name)                               \
    M_SINGLE_MANAGER_DIALOG_NEW2(name, MusicApplication::instance())

#define M_SINGLE_MANAGER_DIALOG_CLASS2(name, parent)                    \
{                                                                       \
    M_SINGLE_MANAGER_DIALOG_NEW2(name, parent)                          \
    w->exec();                                                          \
}

#define M_SINGLE_MANAGER_DIALOG_CLASS(name)                             \
    M_SINGLE_MANAGER_DIALOG_CLASS2(name, MusicApplication::instance())
//
#define M_SINGLE_MANAGER_CORE_NEW2(name, parent)                        \
    MusicSingleManager *manager = M_SINGLE_MANAGER_PTR;                 \
    if(!manager->contains(#name))                                       \
    {                                                                   \
        manager->createObject(#name, new name(parent));                 \
    }                                                                   \
    name *w = MStatic_cast(name*, manager->object(#name));

#define M_SINGLE_MANAGER_CORE_NEW(name)                                 \
    M_SINGLE_MANAGER_CORE_NEW2(name, nullptr)

#define M_SINGLE_MANAGER_CORE_CLASS2(name, parent)                      \
{                                                                       \
    M_SINGLE_MANAGER_CORE_NEW2(name, parent)                            \
    w->start();                                                         \
}

#define M_SINGLE_MANAGER_CORE_CLASS(name)                               \
    M_SINGLE_MANAGER_CORE_CLASS2(name, nullptr)
//


/*! @brief The class of the single object manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSingleManager
{
    TTK_DECLARE_MODULE(MusicSingleManager)
public:
    /*!
     * Add object by type name.
     */
    void createObject(const QString &name, QObject *object);
    /*!
     * Remove object by type name.
     */
    void removeObject(const QString &name);
    /*!
     * Find object by name.
     */
    QObject *object(const QString &name);
    /*!
     * Find object contains by name.
     */
    bool contains(const QString &name) const;

protected:
    QMap<QString, QObject*> m_parameter;

    /*!
     * Object contsructor.
     */
    MusicSingleManager();

    DECLARE_SINGLETON_CLASS(MusicSingleManager)

};

#endif // MUSICSINGLEMANAGER_H
