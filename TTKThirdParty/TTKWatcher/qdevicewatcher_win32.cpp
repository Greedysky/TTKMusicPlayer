#include "qdevicewatcher.h"
#include "qdevicewatcher_p.h"

//TODO: If wince works, use Q_OS_WIN
#ifdef Q_OS_WIN32
#include <QStringList>
#include <QCoreApplication>

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#include <dbt.h>

#ifndef DBT_CUSTOMEVENT
#  define DBT_CUSTOMEVENT 0x8006
#endif

#define CONFIG_NOTIFICATION 0

#if CONFIG_NOTIFICATION
static HDEVNOTIFY hDevNotify = 0;
#define HID_CLASSGUID {0x4d1e55b2, 0xf16f, 0x11cf,{ 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}}
static const GUID WceusbshGUID = { 0x25dbce51, 0x6c8f, 0x4a72, 0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };
static const GUID GUID_DEVINTERFACE_USBSTOR = { 0xA5DCBF10L, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
static const GUID InterfaceClassGuid = GUID_DEVINTERFACE_USBSTOR;//(GUID)HID_CLASSGUID; //GUID_DEVINTERFACE_USBSTOR
#endif //CONFIG_NOTIFICATION

Q_CORE_EXPORT HINSTANCE qWinAppInst();

static QStringList drivesFromMask(quint32 driveBits) //driveBits ->unitmask
{
    QStringList ret;
#if 1  //a disk with multiple partitions removed
    char driveName[] = "A:/";
    driveBits &= 0x3ffffff;
    while (driveBits) {
        if (driveBits & 0x1)
            ret.append(QString::fromLatin1(driveName));
        ++driveName[0];
        driveBits >>= 1;
    }
    return ret;
#else
    char i = 0;
    for (; i<26; ++i) {
        if (driveBits & 0x1)
            break;
        driveBits >>= 1;
    }
    QChar c(i + 'A');
    return ret << QString(c) + ":";
#endif
}

LRESULT CALLBACK dw_internal_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DEVICECHANGE) {
        DEV_BROADCAST_HDR *lpdb = (DEV_BROADCAST_HDR *)lParam;
        qDebug("Device type address: %#x", lpdb);
        if (lpdb) {
            if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                qDebug("DBT_DEVTYP_VOLUME");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) {
                qDebug("DBT_DEVTYP_PORT");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                qDebug("DBT_DEVTYP_DEVICEINTERFACE");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_OEM) {
                qDebug("DBT_DEVTYP_OEM");
            } else {
                qDebug("Unknow device type");
            }
        }

        switch (wParam) {
        case DBT_DEVNODES_CHANGED:
            qDebug("DBT_DEVNODES_CHANGED message received, no extended info.");
            break;
        case DBT_QUERYCHANGECONFIG:
            qDebug("DBT_QUERYCHANGECONFIG message received, no extended info.");
            break;
        case DBT_CONFIGCHANGED:
            qDebug("DBT_CONFIGCHANGED message received, no extended info.");
            break;
        case DBT_CONFIGCHANGECANCELED:
            qDebug("DBT_CONFIGCHANGECANCELED message received, no extended info.");
            break;

        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEQUERYREMOVE:
        case DBT_DEVICEQUERYREMOVEFAILED:
        case DBT_DEVICEREMOVEPENDING:
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
                DEV_BROADCAST_VOLUME *db_volume = (DEV_BROADCAST_VOLUME *)lpdb;
                QStringList drives = drivesFromMask(db_volume->dbcv_unitmask);
#ifdef GWLP_USERDATA
                QDeviceWatcherPrivate *watcher = (QDeviceWatcherPrivate *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
#else
                QDeviceWatcherPrivate *watcher = (QDeviceWatcherPrivate *)GetWindowLong(hwnd, GWL_USERDATA);
#endif
                QList<QDeviceChangeEvent *> events;
                QString action_str("add");
                QDeviceChangeEvent::Action action = QDeviceChangeEvent::Add;
                if (wParam == DBT_DEVICEARRIVAL) {
                } else if (wParam == DBT_DEVICEQUERYREMOVE) {
                } else if (wParam == DBT_DEVICEQUERYREMOVEFAILED) {
                } else if (wParam == DBT_DEVICEREMOVEPENDING) {
                } else if (wParam == DBT_DEVICEREMOVECOMPLETE) {
                    action_str = "remove";
                    action = QDeviceChangeEvent::Remove;
                }
                foreach (const QString &drive, drives) {
                    if (db_volume->dbcv_flags & DBTF_MEDIA)
                        qDebug("Drive %c: Media has been removed.", drive.at(0).toLatin1());
                    else if (db_volume->dbcv_flags & DBTF_NET)
                        qDebug("Drive %c: Network share has been removed.", drive.at(0).toLatin1());
                    else
                        qDebug("Drive %c: Device has been removed.", drive.at(0).toLatin1());
                    watcher->emitDeviceAction(drive, action_str);
                    if (!watcher->event_receivers.isEmpty())
                        events.append(new QDeviceChangeEvent(action, drive));
                }
                if (!events.isEmpty() && !watcher->event_receivers.isEmpty()) {
                    for(QObject *obj : qAsConst(watcher->event_receivers)) {
                        for(QDeviceChangeEvent *event : qAsConst(events)) {
                            QCoreApplication::postEvent(obj, event, Qt::HighEventPriority);
                        }
                    }
                }
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_PORT) {
                qDebug("DBT_DEVTYP_PORT");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                //RegisterDeviceNotification()
                qDebug("DBT_DEVTYP_DEVICEINTERFACE");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_OEM) {
                qDebug("DBT_DEVTYP_OEM");
            } else if (lpdb->dbch_devicetype == DBT_DEVTYP_HANDLE) {
                qDebug("DBT_DEVTYP_HANDLE");
            }
            break;
        case DBT_DEVICETYPESPECIFIC:
            qDebug("DBT_DEVICETYPESPECIFIC message received, may contain an extended info.");
            break;
        case DBT_CUSTOMEVENT:
            qDebug("DBT_CUSTOMEVENT message received, contains an extended info.");
            break;
        case DBT_USERDEFINED:
            qDebug("WM_DEVICECHANGE user defined message received, can not handle.");
            break;
        default:
            qWarning("WM_DEVICECHANGE message received, unhandled value %d.", wParam);
            break;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

static QString className()
{
    return QLatin1String("QDeviceWatcherPrivateWin32_Internal_Widget") + QString::number(quintptr(dw_internal_proc));
}

static HWND dw_create_internal_window(const void* userData)
{
    QString className = ::className();
    HINSTANCE hi = qWinAppInst();

    WNDCLASSW wc;
    wc.style = 0;
    wc.lpfnWndProc = dw_internal_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hi;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = TTKReinterpretCast(const wchar_t *, className.utf16());
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(wc.lpszClassName,       // classname
                             wc.lpszClassName, // window name
                             0,                // style
                             0,
                             0,
                             0,
                             0,  // geometry
                             0,  // parent
                             0,  // menu handle
                             hi, // application
                             0); // windows creation data.
    if (!hwnd) {
        qWarning("QDeviceWatcherPrivate: Failed to create internal window: %d", TTKStaticCast(int, GetLastError()));
#if CONFIG_NOTIFICATION
    } else {
        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
        NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        NotificationFilter.dbcc_classguid = InterfaceClassGuid;
        //3rd param with DEVICE_NOTIFY_ALL_INTERFACE_CLASSES and dbcc_classguid will be ignored
        //http://msdn.microsoft.com/en-us/library/windows/desktop/aa363431(v=vs.85).aspx
        hDevNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
        if (!hDevNotify) {
            qDebug("");
        }
    }
#else
    } else if (userData) {
#ifdef GWLP_USERDATA
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)userData);
#else
        SetWindowLongW(hwnd, GWL_USERDATA, (LONG)userData);
#endif
    }
#endif //CONFIG_NOTIFICATION
    return hwnd;
}

static void dw_destroy_internal_window(HWND hwnd)
{
    if (hwnd)
        DestroyWindow(hwnd);

#if CONFIG_NOTIFICATION
    UnregisterDeviceNotification(hDevNotify);
#endif
    UnregisterClassW(TTKReinterpretCast(const wchar_t *, className().utf16()), qWinAppInst());
}


QDeviceWatcherPrivate::~QDeviceWatcherPrivate()
{
    stop();
}

bool QDeviceWatcherPrivate::start()
{
    if (!initialize())
        return false;
    hwnd = dw_create_internal_window(this);

    if (!hwnd) {
        dw_destroy_internal_window(hwnd);
    }
    return hwnd;
}

bool QDeviceWatcherPrivate::stop()
{
    dw_destroy_internal_window(hwnd);
    return true;
}

bool QDeviceWatcherPrivate::initialize()
{
    return true;
}

void QDeviceWatcherPrivate::parseDeviceInfo()
{
}

#endif //Q_OS_WIN32
