#ifndef QDEVICEWATCHER_P_H
#define QDEVICEWATCHER_P_H

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

#include <QtGlobal>
#define CONFIG_SOCKETNOTIFIER 1
#define CONFIG_TCPSOCKET 0  //QtNetwork

#if defined(Q_OS_WINCE)
#  define CONFIG_THREAD 1
#elif defined(Q_OS_LINUX)
#  define CONFIG_THREAD (!CONFIG_SOCKETNOTIFIER && !CONFIG_TCPSOCKET)
#elif defined Q_OS_MAC //OSX or MACX
#  define CONFIG_THREAD 1
#  include <DiskArbitration/DiskArbitration.h>
#else
#  define CONFIG_THREAD 0
#endif

#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <qt_windows.h>
#else
#  include <QBuffer>
#endif //Q_OS_WIN
#include <QList>
#include <QThread>
#include "ttkprivate.h"

class QDeviceWatcher;

/*! @brief The class of the device watcher private.
 * @author WangBin <wbsecg1@gmail.com>
 */
class TTK_MODULE_EXPORT QDeviceWatcherPrivate
#if CONFIG_THREAD
    : public QThread
#else
    : public QObject
#endif //CONFIG_THREAD
{
    Q_OBJECT
public:
    QDeviceWatcherPrivate(QObject *parent = nullptr) :
#if CONFIG_THREAD
    QThread(parent)
#else
    QObject(parent)
#endif //CONFIG_THREAD
    {
        //initialize();
    }
    ~QDeviceWatcherPrivate();

    void setWatcher(QDeviceWatcher *w) {watcher=w;}
    bool start(); //conflict with QThread::start()
    bool stop();

    //Do not use Qt::DirectConnection. this thread is not watcher's thread!
    void emitDeviceAdded(const QString& dev);
    void emitDeviceChanged(const QString& dev); //Linux: when umounting the device
    void emitDeviceRemoved(const QString& dev);
    void emitDeviceAction(const QString& dev, const QString& action);

    QObjectList event_receivers;

private Q_SLOTS:
    void parseDeviceInfo();

private:
    QDeviceWatcher *watcher;

    bool initialize();
#if CONFIG_THREAD
    virtual void run();
#endif //CONFIG_THREAD
#if defined(Q_OS_LINUX)
    QBuffer buffer;
    void parseLine(const QByteArray &line);
#  if CONFIG_TCPSOCKET
    class QTcpSocket *tcp_socket;
#  elif CONFIG_SOCKETNOTIFIER
    class QSocketNotifier *socket_notifier;
#  endif

    QString bus_name;
    int netlink_socket;
#elif defined(Q_OS_WIN32)
    HWND hwnd;
#elif defined(Q_OS_WINCE)
    HANDLE mQueueHandle;
    HANDLE mNotificationHandle;
#endif
#ifdef Q_OS_MAC
    volatile bool mStop;
    DASessionRef mSession;
#endif //Q_OS_MAC
};

#endif // QDEVICEWATCHER_P_H
