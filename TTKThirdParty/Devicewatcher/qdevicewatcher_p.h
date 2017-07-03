/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#ifndef QDEVICEWATCHER_P_H
#define QDEVICEWATCHER_P_H

#include <QtCore/qglobal.h>
#define CONFIG_SOCKETNOTIFIER 1
#define CONFIG_TCPSOCKET 0  //QtNetwork

#if defined(Q_OS_WINCE)
#define CONFIG_THREAD 1
#elif defined(Q_OS_LINUX)
#define CONFIG_THREAD (!CONFIG_SOCKETNOTIFIER && !CONFIG_TCPSOCKET)
#elif defined Q_OS_MAC //OSX or MACX
#define CONFIG_THREAD 1
#include <DiskArbitration/DiskArbitration.h>
#else
#define CONFIG_THREAD 0
#endif

//#define QT_NO_DEBUG_OUTPUT 0
#define CONFIG_DEBUG 1
#if CONFIG_DEBUG
#define zDebug(fmt, ...) qDebug("%s: "#fmt, __FUNCTION__, ##__VA_ARGS__)
#else
#define zDebug(fmt, ...)
#endif //CONFIG_DEBUG

#ifdef Q_OS_WIN
#include <qt_windows.h>
#else
#include <QtCore/QBuffer>
#endif //Q_OS_WIN
#include <QtCore/QList>
#include <QtCore/QThread>

class QDeviceWatcher;
class QDeviceWatcherPrivate
#if CONFIG_THREAD
		: public QThread
#else
		: public QObject
#endif //CONFIG_THREAD
{
	Q_OBJECT
public:
	QDeviceWatcherPrivate(QObject *parent = 0) :
#if CONFIG_THREAD
	QThread(parent)
#else
	QObject(parent)
#endif //CONFIG_THREAD
	{
		//init();
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

	QList<QObject*> event_receivers;

private Q_SLOTS:
	void parseDeviceInfo();

private:
	QDeviceWatcher *watcher;

	bool init();
#if CONFIG_THREAD
	virtual void run();
#endif //CONFIG_THREAD
#if defined(Q_OS_LINUX)
	QBuffer buffer;
	void parseLine(const QByteArray& line);
# if CONFIG_TCPSOCKET
	class QTcpSocket *tcp_socket;
# elif CONFIG_SOCKETNOTIFIER
	class QSocketNotifier *socket_notifier;
# endif

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
