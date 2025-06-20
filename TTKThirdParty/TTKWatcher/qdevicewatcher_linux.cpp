#include "qdevicewatcher.h"
#include "qdevicewatcher_p.h"

#ifdef Q_OS_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>

#include <QtCore/QCoreApplication>
#include <QtCore/qregexp.h>
#if CONFIG_SOCKETNOTIFIER
#  include <QtCore/QSocketNotifier>
#elif CONFIG_TCPSOCKET
#  include <QtNetwork/QTcpSocket>
#endif


#define UEVENT_BUFFER_SIZE      2048

enum udev_monitor_netlink_group {
    UDEV_MONITOR_NONE,
    UDEV_MONITOR_KERNEL,
    UDEV_MONITOR_UDEV
};

QDeviceWatcherPrivate::~QDeviceWatcherPrivate()
{
    stop();
    close(netlink_socket);
    netlink_socket = -1;
}

bool QDeviceWatcherPrivate::start()
{
    if (!initialize())
        return false;
#if CONFIG_SOCKETNOTIFIER
    socket_notifier->setEnabled(true);
#elif CONFIG_TCPSOCKET
    connect(tcp_socket, SIGNAL(readyRead()), SLOT(parseDeviceInfo()));
#else
    this->QThread::start();
#endif
    return true;
}

bool QDeviceWatcherPrivate::stop()
{
    if (netlink_socket!=-1) {
#if CONFIG_SOCKETNOTIFIER
        socket_notifier->setEnabled(false);
#elif CONFIG_TCPSOCKET
        //tcp_socket->close(); //how to restart?
        disconnect(this, SLOT(parseDeviceInfo()));
#else
        this->quit();
#endif
        close(netlink_socket);
        netlink_socket = -1;
    }
    return true;
}


void QDeviceWatcherPrivate::parseDeviceInfo()
{
    QByteArray data;
#if CONFIG_SOCKETNOTIFIER
    //socket_notifier->setEnabled(false); //for win
    data.resize(UEVENT_BUFFER_SIZE * 2);
    data.fill(0);
    size_t len = read(socket_notifier->socket(), data.data(), UEVENT_BUFFER_SIZE * 2);
    qDebug("read fro socket %d bytes", len);
    data.resize(len);
    //socket_notifier->setEnabled(true); //for win
#elif CONFIG_TCPSOCKET
    data = tcp_socket->readAll();
#endif
    data = data.replace(0, '\n').trimmed(); //In the original line each information is seperated by 0
    if (buffer.isOpen())
        buffer.close();
    buffer.setBuffer(&data);
    if (!buffer.open(QIODevice::ReadOnly))
        return;
    while(!buffer.atEnd()) { //buffer.canReadLine() always false?
        parseLine(buffer.readLine().trimmed());
    }
    buffer.close();
}

#if CONFIG_THREAD
//another thread
void QDeviceWatcherPrivate::run()
{
    QByteArray data;
    //loop only when event happens. because of recv() block the function?
    while (1) {
        //char buf[UEVENT_BUFFER_SIZE * 2] = {0};
        //recv(d->netlink_socket, &buf, sizeof(buf), 0);
        data.resize(UEVENT_BUFFER_SIZE * 2);
        data.fill(0);
        size_t len = recv(netlink_socket, data.data(), data.length(), 0);
        qDebug("read fro socket %d bytes", len);
        data.resize(len);
        data = data.replace(0, '\n').trimmed();
        if (buffer.isOpen())
            buffer.close();
        buffer.setBuffer(&data);
        if (buffer.open(QIODevice::ReadOnly))
            return;
        QByteArray line = buffer.readLine();
        while(!line.isNull()) {
            parseLine(line.trimmed());
            line = buffer.readLine();
        }
        buffer.close();
    }
}
#endif //CONFIG_THREAD

/**
 * Create new udev monitor and connect to a specified event
 * source. Valid sources identifiers are "udev" and "kernel".
 *
 * Applications should usually not connect directly to the
 * "kernel" events, because the devices might not be useable
 * at that time, before udev has configured them, and created
 * device nodes.
 *
 * Accessing devices at the same time as udev, might result
 * in unpredictable behavior.
 *
 * The "udev" events are sent out after udev has finished its
 * event processing, all rules have been processed, and needed
 * device nodes are created.
 **/

bool QDeviceWatcherPrivate::initialize()
{
    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024 * 1024;
    int retval;

    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_groups = UDEV_MONITOR_KERNEL;

    netlink_socket = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    //netlink_socket = socket(PF_NETLINK, SOCK_DGRAM|SOCK_CLOEXEC, NETLINK_KOBJECT_UEVENT); //SOCK_CLOEXEC may be not available
    if (netlink_socket == -1) {
        qWarning("error getting socket: %s", strerror(errno));
        return false;
    }

    /* set receive buffersize */
    setsockopt(netlink_socket, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    retval = bind(netlink_socket, (struct sockaddr*) &snl, sizeof(struct sockaddr_nl));
    if (retval < 0) {
        qWarning("bind failed: %s", strerror(errno));
        close(netlink_socket);
        netlink_socket = -1;
        return false;
    } else if (retval == 0) {
        //from libudev-monitor.c
        struct sockaddr_nl _snl;
        socklen_t _addrlen;

        /*
         * get the address the kernel has assigned us
         * it is usually, but not necessarily the pid
         */
        _addrlen = sizeof(struct sockaddr_nl);
        retval = getsockname(netlink_socket, (struct sockaddr *)&_snl, &_addrlen);
        if (retval == 0)
            snl.nl_pid = _snl.nl_pid;
    }

#if CONFIG_SOCKETNOTIFIER
    socket_notifier = new QSocketNotifier(netlink_socket, QSocketNotifier::Read, this);
    connect(socket_notifier, SIGNAL(activated(int)), SLOT(parseDeviceInfo())); //will always active
    socket_notifier->setEnabled(false);
#elif CONFIG_TCPSOCKET
    //QAbstractSocket *socket = new QAbstractSocket(QAbstractSocket::UnknownSocketType, this); //will not detect "remove", why?
    tcp_socket = new QTcpSocket(this); //works too
    if (!tcp_socket->setSocketDescriptor(netlink_socket, QAbstractSocket::ConnectedState)) {
        qWarning("Failed to assign native socket to QAbstractSocket: %s", qPrintable(tcp_socket->errorString()));
        delete tcp_socket;
        return false;
    }
#endif
    return true;
}

void QDeviceWatcherPrivate::parseLine(const QByteArray &line)
{
    qDebug("%s", line.constData());
#define USE_REGEXP 0
#if USE_REGEXP
    const QRegExp rx("(\\w+)(?:@/.*/block/.*/)(\\w+)\\W*");
    //const QRegExp rx("(add|remove|change)@/.*/block/.*/(\\w+)\\W*");
    if (rx.indexIn(line) == -1)
        return;
    QString action_str = rx.cap(1).toLower();
    QString dev = "/dev/" + rx.cap(2);
#else
    if (!line.contains("/block/")) //hotplug
        return;
    QString action_str = line.left(line.indexOf('@')).toLower();
    QString dev = "/dev/" + line.right(line.length() - line.lastIndexOf('/') - 1);
#endif //USE_REGEXP
    QDeviceChangeEvent *event = 0;

    if (action_str==QLatin1String("add")) {
        emitDeviceAdded(dev);
        event = new QDeviceChangeEvent(QDeviceChangeEvent::Add, dev);
    } else if (action_str==QLatin1String("remove")) {
        emitDeviceRemoved(dev);
        event = new QDeviceChangeEvent(QDeviceChangeEvent::Remove, dev);
    } else if (action_str==QLatin1String("change")) {
        emitDeviceChanged(dev);
        event = new QDeviceChangeEvent(QDeviceChangeEvent::Change, dev);
    }

    qDebug("%s %s", qPrintable(action_str), qPrintable(dev));

    if (event != 0 && !event_receivers.isEmpty()) {
        for(QObject *obj : qAsConst(event_receivers)) {
            QCoreApplication::postEvent(obj, event, Qt::HighEventPriority);
        }
    }
}

#endif //Q_OS_LINUX
