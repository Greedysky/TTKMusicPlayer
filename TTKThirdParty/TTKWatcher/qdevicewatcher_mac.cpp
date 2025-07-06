#include "qdevicewatcher_p.h"
#include <QtCore/QStringList>

#include <CoreServices/CoreServices.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IODVDMedia.h>

static QStringList sDevices; //disk list, or mount point list?

static void onDiskAppear(DADiskRef disk, void *context)
{
    QString disk_name = DADiskGetBSDName(disk);
    if (sDevices.contains(disk_name))
        return;
    sDevices.append(disk_name);
    QDeviceWatcherPrivate *p = TTKStaticCast(QDeviceWatcherPrivate*, context);
    p->emitDeviceAdded(disk_name);
}

static void onDiskDisappear(DADiskRef disk, void *context)
{
    QString disk_name = DADiskGetBSDName(disk);
    sDevices.removeAll(disk_name); //erase?
    QDeviceWatcherPrivate *p = TTKStaticCast(QDeviceWatcherPrivate*, context);
    p->emitDeviceRemoved(disk_name);
}

QDeviceWatcherPrivate::~QDeviceWatcherPrivate()
{
    stop();
}

bool QDeviceWatcherPrivate::start()
{
    initialize();
    QThread::start();
}

bool QDeviceWatcherPrivate::stop()
{
    mStop = true;
    wait();
    //DAUnregisterApprovalCallback
    DAUnregisterCallback(mSession, (void*)onDiskAppear, this);
    DAUnregisterCallback(mSession, (void*)onDiskDisappear, this);
}

void QDeviceWatcherPrivate::parseDeviceInfo()
{
}

bool QDeviceWatcherPrivate::initialize()
{
    //get sDevices
    //FSGetVolumeInfo()
    mSession = DASessionCreate(kCFAllocatorDefault);
    if (!mSession) {
        qWarning("Failed to create Disk Arbitration session");
        return false;
    }

    DARegisterDiskAppearedCallback(mSession, nullptr, onDiskAppear, this);
    DARegisterDiskDisappearedCallback(mSession, nullptr, onDiskDisappear, this);
}

void QDeviceWatcherPrivate::run()
{
    mStop = false;

    DASessionScheduleWithRunLoop(mSession, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    SInt32 result;
    do {
        result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, true);
    } while (!mStop && result);

    DASessionUnscheduleFromRunLoop(mSession, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
}
