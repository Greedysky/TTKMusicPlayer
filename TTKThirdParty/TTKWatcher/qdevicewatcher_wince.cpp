#include "qdevicewatcher.h"
#include "qdevicewatcher_p.h"

#ifdef Q_OS_WINCE
#include <msgqueue.h>
#include <pnp.h>
#include <Storemgr.h> //BLOCK_DRIVER_GUID
#include <QtCore/QCoreApplication>

#ifdef UNICODE
#  if TTK_QT_VERSION_CHECK(4,0,0)
#    define TCHAR2QString(x) QString::fromUtf16(x)
#  else
#    define TCHAR2QString(x) QString::fromUcs2(x)
#  endif
#else
#  define TCHAR2QString(x)   QString::fromLocal8Bit((x))
#endif //UNICODE

typedef union {
    DEVDETAIL d;
    char pad[sizeof(DEVDETAIL) + MAX_DEVCLASS_NAMELEN]; //BYTE pad[sizeof(DEVDETAIL) + (MAX_PATH * sizeof(TCHAR))];
} Q_DEVDETAIL;


QDeviceWatcherPrivate::~QDeviceWatcherPrivate()
{
}

bool QDeviceWatcherPrivate::start()
{
    if (!initialize())
        return false;
    this->QThread::start();
    return true;
}


bool QDeviceWatcherPrivate::stop()
{
    quit();

    if (!StopDeviceNotifications(mNotificationHandle)) {
        qWarning("StopDeviceNotifications() error: %d", GetLastError());
        return false;
    }
    if (!CloseMsgQueue(mQueueHandle)) {
        qWarning("CloseMsgQueue() error: %d", GetLastError());
        return false;
    }
    return true;
}


bool QDeviceWatcherPrivate::initialize()
{
    MSGQUEUEOPTIONS msgopts;

    msgopts.dwSize = sizeof(MSGQUEUEOPTIONS);
    msgopts.dwFlags = 0; //MSGQUEUE_VARIABLESIZE | MSGQUEUE_MSGSIZE; /*msdn*/ //MSGQUEUE_NOPRECOMMIT | MSGQUEUE_ALLOW_BROKEN
    msgopts.dwMaxMessages = 0;
    msgopts.cbMaxMessage = sizeof(Q_DEVDETAIL);
    msgopts.bReadAccess = TRUE;
    msgopts.dwMaxMessages = 0; //?
    //msgopts.cbMaxMsgQueue = 0; //?
    //msgopts.dwDesiredAccess = GENERIC_READ;
    //msgopts.dwShareMode = 0;
    //msgopts.dwCreationDisposition = CREATE_NEW;

    mQueueHandle = CreateMsgQueue(nullptr, &msgopts);
    if (mQueueHandle == nullptr) {
        qWarning("CreateMsgQueue() error: %d", GetLastError());
        return false;
    }
    mNotificationHandle = RequestDeviceNotifications(&BLOCK_DRIVER_GUID, mQueueHandle, TRUE);
    if (mNotificationHandle == nullptr) {
        qWarning("RequestDeviceNotifications() error: %d", GetLastError());
        return false;
    }
    return true;
}

void QDeviceWatcherPrivate::run()
{
    DWORD flags;
    DWORD size;
    Q_DEVDETAIL detail;
    SetLastError(0); //?
    while (true) {
        if(WaitForSingleObject(mQueueHandle, 3000) == WAIT_OBJECT_0) {
            while(ReadMsgQueue(mQueueHandle, &detail, sizeof(detail), &size, 1, &flags)) {
                QString dev = TCHAR2QString(detail.d.szName);
                QDeviceChangeEvent *event = 0;
                if (detail.d.fAttached) {
                    emitDeviceAdded(dev);
                    event = new QDeviceChangeEvent(QDeviceChangeEvent::Add, dev);
                } else {
                    emitDeviceRemoved(dev);
                    event = new QDeviceChangeEvent(QDeviceChangeEvent::Remove, dev);
                }
                if (event != 0 && !event_receivers.isEmpty()) {
                    for(QObject *obj : qAsConst(event_receivers)) {
                        QCoreApplication::postEvent(obj, event, Qt::HighEventPriority);
                    }
                }
            }
        }
    }
}

void QDeviceWatcherPrivate::parseDeviceInfo()
{
}

#endif //Q_OS_WINCE
