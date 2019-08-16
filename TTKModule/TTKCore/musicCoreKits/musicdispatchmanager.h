#ifndef MUSICDISPATCHMANAGER_H
#define MUSICDISPATCHMANAGER_H

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
#include "musicsingleton.h"

#define M_DISPATCH_PTR (MusicSingleton<MusicDispatchManager>::createInstance())

class QTimer;

/*! @brief The class of the functions dispatch item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicDispatchItem
{
    int m_type;
    int m_times;
    QList<QVariant> m_args;
    MusicDispatchItem()
    {
        m_times = 0;
        m_type = -1;
    }

    bool isEmpty() const
    {
        return m_args.isEmpty();
    }

    bool isValid() const
    {
        return m_times <= 5;
    }

}MusicDispatchItem;


/*! @brief The class of the functions dispatch manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicDispatchManager : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDispatchManager)
public:
    /*!
     * Set dispatch.
     */
    void dispatch(int type);
    void dispatch(int type, const QVariant &arg1);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4);
    void dispatch(int type, const QList<QVariant> &args);
    void dispatch(int type, void *funcs);


private Q_SLOTS:
    /*!
     * Active functions.
     */
    void activeFunctions();

protected:
    /*!
     * Object contsructor.
     */
    MusicDispatchManager();

    ~MusicDispatchManager();

    QMutex m_mutex;
    QTimer *m_timer;
    QList<MusicDispatchItem*> m_observer;

    DECLARE_SINGLETON_CLASS(MusicDispatchManager)

};

#endif // MUSICDISPATCHMANAGER_H
