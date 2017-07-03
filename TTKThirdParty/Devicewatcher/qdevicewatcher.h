/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#ifndef QDEVICEWATCHER_H
#define QDEVICEWATCHER_H

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include "musicextrasglobaldefine.h"

class QDeviceWatcherPrivate;

class MUSIC_EXTRAS_EXPORT QDeviceWatcher : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(QDeviceWatcher)
public:
	explicit QDeviceWatcher(QObject *parent = 0);
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


class MUSIC_EXTRAS_EXPORT QDeviceChangeEvent : public QEvent
{
public:
    enum Action { Add, Remove, Change};
    //static const Type EventType; //VC link error

    explicit QDeviceChangeEvent(Action action, const QString& device);

    Action action() const {return m_action;}
    QString device() const {return m_device;}
    static Type registeredType()
    {
        static Type EventType = static_cast<Type>(registerEventType());
        return EventType;
    }

private:
    Action m_action;
    QString m_device;
};

#endif // QDEVICEWATCHER_H
