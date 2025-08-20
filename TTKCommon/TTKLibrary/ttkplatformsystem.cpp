#include "ttkplatformsystem.h"

#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <qt_windows.h>
#elif defined Q_OS_LINUX
#  include <QFile>
#  include "ttkregularexpression.h"
#endif

TTKPlatformSystem::System TTKPlatformSystem::systemName()
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
        TTKRegularExpression regx("DISTRIB_ID=(\\w+)");
        if(regx.match(QString(file.readAll())) != -1)
        {
            const QString &system = regx.captured(1).toLower();

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
#elif defined Q_OS_MAC
    return System::Mac;
#endif
    return System::Unkown;
}
