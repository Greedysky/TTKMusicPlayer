#ifndef TTKDISPATCHMANAGER_H
#define TTKDISPATCHMANAGER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QMutex>
#include "ttksingleton.h"

class QTimer;
struct TTKDispatchItem;

/*! @brief The class of the functions dispatch manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKDispatchManager : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKDispatchManager)
public:
    enum class Module
    {
        Null = -1,
        FileRemove
    };

    /*!
     * Set dispatch.
     */
    void dispatch(Module type);
    void dispatch(Module type, void *funcs);
    void dispatch(Module type, const TTKVariantList &args);

public:
    /*!
     * Set dispatch.
     */
    void dispatch(Module type, const QVariant &arg1);
    void dispatch(Module type, const QVariant &arg1, const QVariant &arg2);
    void dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3);
    void dispatch(Module type, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4);

private Q_SLOTS:
    /*!
     * Active functions.
     */
    void activeFunctions();

private:
    /*!
     * Object constructor.
     */
    TTKDispatchManager();
    /*!
     * Object destructor.
     */
    ~TTKDispatchManager();

    QMutex m_mutex;
    QTimer *m_timer;
    QList<TTKDispatchItem*> m_observer;

    TTK_DECLARE_SINGLETON_CLASS(TTKDispatchManager)

};


/*! @brief The class of the functions dispatch item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT TTKDispatchItem
{
    int m_times;
    TTKDispatchManager::Module m_type;
    TTKVariantList m_args;

    TTKDispatchItem()
        : m_times(0),
          m_type(TTKDispatchManager::Module::Null)
    {

    }

    inline bool isEmpty() const
    {
        return m_args.isEmpty();
    }

    inline bool isValid() const
    {
        return m_times <= 5;
    }
};

#endif // TTKDISPATCHMANAGER_H
