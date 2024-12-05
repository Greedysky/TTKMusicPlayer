#ifndef QDEVICEWATCHER_H
#define QDEVICEWATCHER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include "ttkprivate.h"

class QDeviceWatcherPrivate;

/*! @brief The class of the device watcher.
 * @author WangBin <wbsecg1@gmail.com>
 */
class TTK_MODULE_EXPORT QDeviceWatcher : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QDeviceWatcher)
public:
    explicit QDeviceWatcher(QObject *parent = nullptr);
    ~QDeviceWatcher();

    bool start();
    bool stop();
    bool isRunning() const;

    void appendEventReceiver(QObject* receiver);

Q_SIGNALS:
    void deviceAdded(const QString& dev);
    void deviceChanged(bool state);
    void deviceChanged(const QString& dev); //when umounting the device
    void deviceRemoved(const QString& dev);

protected:
    bool running;
    QDeviceWatcherPrivate *d_ptr;
};

/*! @brief The class of the device watcher event.
 * @author WangBin <wbsecg1@gmail.com>
 */
class TTK_MODULE_EXPORT QDeviceChangeEvent : public QEvent
{
public:
    enum Action { Add, Remove, Change};
    //static const Type EventType; //VC link error

    explicit QDeviceChangeEvent(Action action, const QString& device);

    Action action() const { return m_action; }
    QString device() const { return m_device; }

    static Type registeredType()
    {
        static Type EventType = TTKStaticCast(Type, registerEventType());
        return EventType;
    }

private:
    Action m_action;
    QString m_device;
};

#endif // QDEVICEWATCHER_H
