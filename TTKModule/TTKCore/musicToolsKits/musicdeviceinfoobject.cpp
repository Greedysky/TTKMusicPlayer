#include "musicdeviceinfoobject.h"
#include "musicsemaphoreloop.h"

#ifdef Q_OS_WIN
#  include <QDir>
#  include <QFileInfoList>
#  include <qt_windows.h>
#endif
#define BUFFER_SIZE 255

MusicDeviceInfoObject::MusicDeviceInfoObject(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_UNIX
    m_dfProcess = new QProcess(this);
    connect(m_dfProcess, SIGNAL(readyRead()), this, SLOT(readData()));
#endif
}

MusicDeviceInfoObject::~MusicDeviceInfoObject()
{
#ifdef Q_OS_UNIX
    delete m_dfProcess;
#endif
}

bool MusicDeviceInfoObject::getDisksProperty(const QString &drive) const
{
#ifdef Q_OS_WIN
    STORAGE_PROPERTY_QUERY query;
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    QString name = drive;
    name = "\\\\.\\" + name.left(name.size() - 1);

    HANDLE hDevice = CreateFileW(
        name.toStdWString().c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    if(hDevice == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    STORAGE_DESCRIPTOR_HEADER *pHeader = (STORAGE_DESCRIPTOR_HEADER *)malloc(sizeof(STORAGE_DESCRIPTOR_HEADER));
    if(!pHeader)
    {
        return false;
    }

    DWORD bytes = 0;
    BOOL bResult = DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        pHeader,
        sizeof(STORAGE_DESCRIPTOR_HEADER),
        &bytes,
        nullptr);

    if(!bResult)
    {
        free(pHeader);
        CloseHandle(hDevice);
        return false;
    }

    PSTORAGE_DEVICE_DESCRIPTOR pDev = (STORAGE_DEVICE_DESCRIPTOR *)malloc(pHeader->Size);
    bResult = DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        pDev,
        pHeader->Size,
        &bytes,
        nullptr);

    free(pHeader);
    CloseHandle(hDevice);
    if(pDev->BusType==BusTypeUsb)
    {
        free(pDev);
        return true;
    }

    free(pDev);
    return false;
#else
    Q_UNUSED(drive);
    return false;
#endif
}

MusicDeviceInfoItems MusicDeviceInfoObject::getRemovableDrive()
{
    m_items.clear();
#ifdef Q_OS_WIN
    const QFileInfoList &drives = QDir::drives();
    for(const QFileInfo &drive : qAsConst(drives))
    {
        const QString &path = drive.absoluteDir().absolutePath();
        const int type = GetDriveTypeW(path.toStdWString().c_str());
        if(type == DRIVE_REMOVABLE)
        {

        }
        else if(type == DRIVE_FIXED)
        {
            if(!getDisksProperty(path))
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        ULARGE_INTEGER freeAvailable;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;

        if(GetDiskFreeSpaceExW(path.toStdWString().c_str(), &freeAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            MusicDeviceInfoItem item;
            item.m_path = path;
            item.m_availableBytes = freeAvailable.QuadPart / MH_GB2B; //GB
            item.m_totalBytes = totalNumberOfBytes.QuadPart / MH_GB2B; //GB
            item.m_availableBytes = item.m_availableBytes >= item.m_totalBytes ? item.m_totalBytes : item.m_totalBytes - item.m_availableBytes;

            DWORD serialNumber = 0;
            DWORD maxLength = 0;
            DWORD fileSysFlags = 0;

            WCHAR dirveName[BUFFER_SIZE];
            WCHAR fileSysTypeName[BUFFER_SIZE];

            if(GetVolumeInformationW(
                item.m_path.toStdWString().c_str(),
                dirveName,
                BUFFER_SIZE,
                &serialNumber,
                &maxLength,
                &fileSysFlags,
                fileSysTypeName,
                BUFFER_SIZE))
            {
                item.m_name = QString::fromStdWString(dirveName);
            }
            m_items << item;
        }
    }
#else
    MusicSemaphoreLoop loop;
    m_dfProcess->close();
    m_dfProcess->start("df -h");
    QObject::connect(m_dfProcess, SIGNAL(finished(int)), &loop, SLOT(quit()));
    QObject::connect(m_dfProcess, SIGNAL(error(QProcess::ProcessError)), &loop, SLOT(quit()));
    loop.exec();
#endif
    return m_items;
}

#ifdef Q_OS_UNIX
void MusicDeviceInfoObject::readData()
{
    while(!m_dfProcess->atEnd())
    {
        const QString &result = QLatin1String(m_dfProcess->readLine());
        if(result.startsWith("/dev/sd"))
        {
            QString dev, use, all, path;
            int index = 0;

            const QStringList &list = result.split(" ");
            for(int i = 0; i < list.count(); i++)
            {
                const QString &data = list.at(i).trimmed();
                if(data.isEmpty())
                {
                    continue;
                }

                index++;
                if(index == 1)
                {
                    dev = data;
                }
                else if(index == 2)
                {
                    all = data;
                    all.remove("G");
                }
                else if(index == 3)
                {
                    use = data;
                    use.remove("G");
                }
                else if(index == 6)
                {
                    path = data;
                }
            }

            if(path.startsWith("/media/"))
            {
                MusicDeviceInfoItem item;
                item.m_name = dev;
                item.m_path = path;
                item.m_availableBytes = use.toDouble();
                item.m_totalBytes = all.toDouble();
                item.m_availableBytes = item.m_availableBytes >= item.m_totalBytes ? item.m_totalBytes : item.m_totalBytes - item.m_availableBytes;

                m_items << item;
            }
        }
    }
}
#endif
