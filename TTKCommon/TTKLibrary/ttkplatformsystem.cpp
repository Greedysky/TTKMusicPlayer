#include "ttkplatformsystem.h"

#ifdef Q_OS_WIN
#  include <qt_windows.h>
#  include <QScreen>
#  include <QApplication>
#elif defined Q_OS_UNIX
#  include <X11/Xlib.h>
#endif
#include <QSize>
#include <QFile>
#include <QRegExp>

static constexpr int DEFAULT_DPI = 96;

static QSize generateDPIValue()
{
    const QSize defaultSize(DEFAULT_DPI, DEFAULT_DPI);
#ifdef Q_OS_WIN
#  if TTK_QT_VERSION_CHECK(5,0,0)
    if(!qApp)
    {
        int count = 0;
        QApplication(count, nullptr);
    }

    QScreen *screen = QApplication::primaryScreen();
    if(!screen)
    {
        return defaultSize;
    }

    const double x = screen->logicalDotsPerInchX();
    const double y = screen->logicalDotsPerInchY();

    return QSize(x, y);
#  endif
#elif defined Q_OS_UNIX
    Display *dp = XOpenDisplay(nullptr);
    if(!dp)
    {
        return defaultSize;
    }

    const int screen = 0; /* Screen number */
    const double x = (DisplayWidth(dp, screen) * 25.4) / DisplayWidthMM(dp, screen);
    const double y = (DisplayHeight(dp, screen) * 25.4) / DisplayHeightMM(dp, screen);

    XCloseDisplay(dp);
    return QSize(x + 0.5, y + 0.5);
#endif
    return defaultSize;
}

int TTKPlatformSystem::logicalDotsPerInchX() const
{
    const QSize dpi(generateDPIValue());
    return dpi.width();
}

int TTKPlatformSystem::logicalDotsPerInchY() const
{
    const QSize dpi(generateDPIValue());
    return dpi.height();
}

int TTKPlatformSystem::logicalDotsPerInch() const
{
    const QSize dpi(generateDPIValue());
    return (dpi.width() + dpi.height()) / 2;
}

TTKPlatformSystem::System TTKPlatformSystem::systemName() const
{
#ifdef Q_OS_WIN
    typedef void (__stdcall *NTPROC)(DWORD*, DWORD*, DWORD*);
    HINSTANCE instance = LoadLibraryA("ntdll.dll");
    DWORD major, minor, buildNumber;
    NTPROC proc = TTKVoidCast(NTPROC)GetProcAddress(instance, "RtlGetNtVersionNumbers");
    proc(&major, &minor, &buildNumber);

    if(major == 6 && minor == 1)	//win 7
    {
        return System::Win7;
    }
    else if(major == 6 && minor == 2)	//win 8
    {
        return System::Win8;
    }
    else if(major == 6 && minor == 3)	//win 8.1
    {
        return System::Win81;
    }
    else if(major == 10 && minor == 0)	//win 10 or win 11
    {
        return buildNumber < (0xF0000000 | 22000) ? System::Win10 : System::Win11;
    }
    FreeLibrary(instance);

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if(GetVersionEx((OSVERSIONINFO *)&os))
    {
        switch(os.dwMajorVersion)
        {
            case 4:
                switch(os.dwMinorVersion)
                {
                    case 0:
                        if(os.dwPlatformId == VER_PLATFORM_WIN32_NT)
                        {
                            return System::WinNT40;
                        }
                        else if(os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                        {
                            return System::Win95;
                        }
                        break;
                    case 10: return System::Win98;
                    case 90: return System::WinMe;
                    default: break;
                }
                break;
            case 5:
                switch(os.dwMinorVersion)
                {
                    case 0: return System::Win2000;
                    case 1: return System::WinXP;
                    case 2:
                        if(os.wProductType == VER_NT_WORKSTATION && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                        {
                            return System::WinXPProfessionalEdition;
                        }
                        else if(GetSystemMetrics(SM_SERVERR2) == 0)
                        {
                            return System::WinServer2003;
                        }
                        else if(GetSystemMetrics(SM_SERVERR2) != 0)
                        {
                            return System::WinServer2003R2;
                        }
                    default: break;
                }
                break;
            case 6:
                switch(os.dwMinorVersion)
                {
                    case 0: return os.wProductType == VER_NT_WORKSTATION ? System::WinVista : System::WinServer2008;
                    case 1: return os.wProductType == VER_NT_WORKSTATION ? System::Win7 : System::WinServer2008R2;
                    case 2: return os.wProductType == VER_NT_WORKSTATION ? System::Win8 : System::WinServer2012;
                    default: break;
                }
                break;
            default: return System::Unkown;
        }
    }
#elif defined Q_OS_LINUX
    QFile file("/etc/lsb-release");
    if(file.open(QIODevice::ReadOnly))
    {
        const QRegExp regx("DISTRIB_ID=(\\w+)");
        if(regx.indexIn(QString(file.readAll())) != -1)
        {
            const QString &system = regx.cap(1).toLower();

            if(system == "ubuntu")
            {
                return System::LinuxUbuntu;
            }
            else if(system == "debian")
            {
                return System::LinuxDebian;
            }
            else if(system == "arch")
            {
                return System::LinuxArch;
            }
            else if(system == "centos")
            {
                return System::LinuxCentOS;
            }
        }
        file.close();
    }
    return System::Linux;
#elif defined Q_OS_MACOS
    return System::Mac;
#endif
    return System::Unkown;
}
