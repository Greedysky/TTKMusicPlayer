#ifndef MUSICSINGLEMANAGER_H
#define MUSICSINGLEMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QMap>
#include "ttksingleton.h"
#include "musicwidgetutils.h"

/////////////////////////////////////////////////////////////////////////
#define GENERATE_SINGLE_WIDGET_CLASS(name)                                 \
    GENERATE_SINGLE_WIDGET_PARENT(name, MusicApplication::instance())

#define GENERATE_SINGLE_WIDGET_PARENT(name, widget)                        \
[](QWidget *parent)                                                        \
{                                                                          \
    MusicSingleManager *manager = G_SINGLE_MANAGER_PTR;                    \
    if(!manager->contains(#name))                                          \
    {                                                                      \
        manager->appendObject(#name, new name(parent));                    \
    }                                                                      \
    name *w = TTKObjectCast(name*, manager->object(#name));                \
    TTK::Widget::adjustWidgetPosition(w);                                  \
    w->raise();                                                            \
    w->show();                                                             \
    return w;                                                              \
}(widget)
/////////////////////////////////////////////////////////////////////////


/*! @brief The class of the single object manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSingleManager
{
    TTK_DECLARE_MODULE(MusicSingleManager)
public:
    /*!
     * Add object by type name.
     */
    void appendObject(const QString &name, QObject *object);
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

private:
    /*!
     * Object constructor.
     */
    MusicSingleManager();
    
    QMap<QString, QObject*> m_parameter;

    TTK_DECLARE_SINGLETON_CLASS(MusicSingleManager)

};

#define G_SINGLE_MANAGER_PTR makeMusicSingleManager()
TTK_MODULE_EXPORT MusicSingleManager* makeMusicSingleManager();

#endif // MUSICSINGLEMANAGER_H
