#include "musicdeviceinfomodule.h"
#include "ttksemaphoreloop.h"

#ifdef Q_OS_WIN
#  include <QDir>
#  include <QFileInfoList>
#  include <qt_windows.h>
#endif
static constexpr int BUFFER_SIZE = 255;

MusicDeviceInfoModule::MusicDeviceInfoModule(QObject *parent)
    : QObject(parent)
{
#ifdef Q_OS_UNIX
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
#endif
}

MusicDeviceInfoModule::~MusicDeviceInfoModule()
{
#ifdef Q_OS_UNIX
    delete m_process;
#endif
}

bool MusicDeviceInfoModule::disksProperty(const QString &drive) const
{
#ifdef Q_OS_WIN
    STORAGE_PROPERTY_QUERY query;
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    QString name = drive;
    name = "\\\\.\\" + name.left(name.length() - 1);

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
    DeviceIoControl(
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

    if(pDev->BusType == BusTypeUsb)
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

MusicDeviceInfoItemList MusicDeviceInfoModule::removableDrive()
{
    m_items.clear();
#ifdef Q_OS_WIN
    for(const QFileInfo &fin : QDir::drives())
    {
        const QString &path = fin.absoluteDir().absolutePath();
        const int type = GetDriveTypeW(path.toStdWString().c_str());
        if(type == DRIVE_REMOVABLE)
        {

        }
        else if(type == DRIVE_FIXED)
        {
            if(!disksProperty(path))
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
            const qint64 available = freeAvailable.QuadPart / TTK_SN_GB2B;
            const qint64 total = totalNumberOfBytes.QuadPart / TTK_SN_GB2B;

            MusicDeviceInfoItem item;
            item.m_path = path;
            item.m_usedBytes = total - available;
            item.m_totalBytes = total;

            if(item.m_usedBytes < 0)
            {
                item.m_usedBytes = 0;
            }
            else if(item.m_usedBytes >= item.m_totalBytes)
            {
                item.m_usedBytes = item.m_totalBytes;
            }

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
    TTKSemaphoreLoop loop;
    connect(m_process, SIGNAL(finished(int)), &loop, SLOT(quit()));
    QtProcessVoidConnect(m_process, &loop, quit, TTK_SLOT);
    m_process->close();
    m_process->start("df", {"-h"});
    loop.exec();
#endif
    return m_items;
}

#ifdef Q_OS_UNIX
static int driveSizeToNumber(QString &v)
{
    if(v.endsWith("G"))
    {
        v.remove("G");
        return v.toDouble();
    }
    else if(v.endsWith("M"))
    {
        v.remove("M");
        return 1;
    }
    return 0;
}

void MusicDeviceInfoModule::handleReadyRead()
{
    while(!m_process->atEnd())
    {
        const QString &result = QString::fromLocal8Bit(m_process->readLine());
        if(result.startsWith("/dev/sd"))
        {
            QString dev, use, total, path;
            int index = 0;

            const QStringList &list = result.split(" ");
            for(int i = 0; i < list.count(); ++i)
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
                    total = data;
                }
                else if(index == 3)
                {
                    use = data;
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
                item.m_usedBytes = driveSizeToNumber(use);
                item.m_totalBytes = driveSizeToNumber(total);

                if(item.m_usedBytes < 0)
                {
                    item.m_usedBytes = 0;
                }
                else if(item.m_usedBytes >= item.m_totalBytes)
                {
                    item.m_usedBytes = item.m_totalBytes;
                }
                m_items << item;
            }
        }
    }
}
#endif
