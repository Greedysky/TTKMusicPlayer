#include "qdevicewatcher.h"
#include "qdevicewatcher_p.h"

QDeviceWatcher::QDeviceWatcher(QObject* parent)
    : QObject(parent),
      running(false),
      d_ptr(new QDeviceWatcherPrivate)
{
    Q_D(QDeviceWatcher);
    d->setWatcher(this);
}

QDeviceWatcher::~QDeviceWatcher()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = nullptr;
    }
}

bool QDeviceWatcher::start()
{
    Q_D(QDeviceWatcher);
    if (!d->start()) {
        stop();
        running = false;
    }
    running = true;
    return running;
}

bool QDeviceWatcher::stop()
{
    Q_D(QDeviceWatcher);
    running = !d->stop();
    return !running;
}

bool QDeviceWatcher::isRunning() const
{
    return running;
}

void QDeviceWatcher::appendEventReceiver(QObject *receiver)
{
    Q_D(QDeviceWatcher);
    d->event_receivers.append(receiver);
}

void QDeviceWatcherPrivate::emitDeviceAdded(const QString &dev)
{
    if (!QMetaObject::invokeMethod(watcher, "deviceAdded", Q_ARG(QString, dev)))
        qWarning("invoke deviceAdded failed");

    QMetaObject::invokeMethod(watcher, "deviceChanged", Q_ARG(bool, true));
}

void QDeviceWatcherPrivate::emitDeviceChanged(const QString &dev)
{
    if (!QMetaObject::invokeMethod(watcher, "deviceChanged", Q_ARG(QString, dev)))
        qWarning("invoke deviceChanged failed");
}

void QDeviceWatcherPrivate::emitDeviceRemoved(const QString &dev)
{
    if (!QMetaObject::invokeMethod(watcher, "deviceRemoved", Q_ARG(QString, dev)))
        qWarning("invoke deviceRemoved failed");

    QMetaObject::invokeMethod(watcher, "deviceChanged", Q_ARG(bool, false));
}

void QDeviceWatcherPrivate::emitDeviceAction(const QString &dev, const QString &action)
{
    QString a(action.toLower());
    if (a == QLatin1String("add"))
        emitDeviceAdded(dev);
    else if (a == QLatin1String("remove"))
        emitDeviceRemoved(dev);
    else if (a == QLatin1String("change"))
        emitDeviceChanged(dev);
}

QDeviceChangeEvent::QDeviceChangeEvent(Action action, const QString &device) :
    QEvent(registeredType())
{
    m_action = action;
    m_device = device;
}
