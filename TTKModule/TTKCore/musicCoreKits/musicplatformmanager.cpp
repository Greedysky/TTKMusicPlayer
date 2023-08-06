#include "musicplatformmanager.h"
#include "musicformats.h"

#include <QScreen>
#include <QSettings>
#include <QProcess>
#include <QStringList>
#include <QApplication>

#ifdef Q_OS_WIN
#  include <qt_windows.h>
#elif defined Q_OS_UNIX
#  include <X11/Xlib.h>
#endif
#define DEFAULT_DPI 96

#ifdef Q_OS_WIN
bool MusicPlatformManager::isFileAssociate()
{
    return currentNodeExist(MP3_FILE_SUFFIX);
}

void MusicPlatformManager::setMusicRegeditAssociateFileIcon()
{
    const QStringList &formats = MusicFormats::supportMusicFormats();
    for(int i = 0; i < formats.count(); ++i)
    {
        const QString &format = formats[i];
        if(!currentNodeExist(format))
        {
            createMusicRegedit(format);
        }
    }
}

void MusicPlatformManager::enabledLeftWinMode()
{
    INPUT input[4];
    memset(input, 0, sizeof(input));
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[2].ki.wVk = VK_LWIN;
    input[1].ki.wVk = input[3].ki.wVk = 0x44;
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, input, sizeof(INPUT));
}

int MusicPlatformManager::windowsIEVersion() const
{
    const DWORD versionInfoSize = GetFileVersionInfoSizeW(L"mshtml.dll", nullptr);
    if(versionInfoSize == 0)
    {
        return -1;
    }

    BYTE *data = new BYTE[versionInfoSize];
    if(!GetFileVersionInfoW(L"mshtml.dll", 0, versionInfoSize, data))
    {
        delete[] data;
        return -1;
    }

    const VS_FIXEDFILEINFO *fixedFileInfo = nullptr;
    UINT fixedFileInfoSize = 0;
    if(!VerQueryValueW(data, L"\\", (LPVOID*)&fixedFileInfo, &fixedFileInfoSize))
    {
        delete[] data;
        return -1;
    }

    delete[] data;
    return HIWORD(fixedFileInfo->dwProductVersionMS);
}

static constexpr const char *GetAutoStartRegKeyPath()
{
    return "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
}

static constexpr const char *GetAutoStartWowRegKeyPath()
{
    return "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
}

void MusicPlatformManager::windowsStartUp(bool v) const
{
    const QString &name = QApplication::applicationName();
    const QString &path = QApplication::applicationFilePath().replace("/", "\\");

    QSettings reg(GetAutoStartRegKeyPath(), QSettings::NativeFormat);
    QSettings regWOW(GetAutoStartWowRegKeyPath(), QSettings::NativeFormat);

    if (v)
    {
        if (reg.value(name).toString() != path)
        {
            reg.setValue(name, path);
        }

        if (regWOW.value(name).toString() != path)
        {
            regWOW.setValue(name, path);
        }
    }
    else
    {
        if (reg.contains(name))
        {
            reg.remove(name);
            reg.sync();
        }

        if (regWOW.contains(name))
        {
            regWOW.remove(name);
            regWOW.sync();
        }
    }
}
#endif

static QSize generateDPIValue()
{
    const QSize defaultSize(DEFAULT_DPI, DEFAULT_DPI);
#ifdef Q_OS_WIN
  #if TTK_QT_VERSION_CHECK(5,0,0)
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
  #endif
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

int MusicPlatformManager::logicalDotsPerInchX() const
{
    const QSize dpi(generateDPIValue());
    return dpi.width();
}

int MusicPlatformManager::logicalDotsPerInchY() const
{
    const QSize dpi(generateDPIValue());
    return dpi.height();
}

int MusicPlatformManager::logicalDotsPerInch() const
{
    const QSize dpi(generateDPIValue());
    return (dpi.width() + dpi.height()) / 2;
}

MusicPlatformManager::System MusicPlatformManager::systemName() const
{
#ifdef Q_OS_WIN
    typedef void (__stdcall *NTPROC)(DWORD*, DWORD*, DWORD*);
    HINSTANCE instance = LoadLibraryW(L"ntdll.dll");
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
#else
    return System::Mac;
#endif
    return System::Unkown;
}

#ifdef Q_OS_WIN
bool MusicPlatformManager::currentNodeExist(const QString &key)
{
    bool state = false;
    const QString &keyX = "HKEY_CURRENT_USER\\Software\\Classes\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    state = (keyXSetting.value("Default").toString() == APP_DOT_NAME + key);

    const QString &fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    state &= (fileExtsSetting.value("Progid").toString() == APP_DOT_NAME + key);

    const QString &fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    state &= (fileExtsUserSetting.value("Progid").toString() == APP_DOT_NAME + key);

    return state;
}

void MusicPlatformManager::createMusicRegedit(const QString &key)
{
    QString keyX = "HKEY_CURRENT_USER\\Software\\Classes\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    keyX = keyXSetting.value("Default").toString();
    if(keyX.isEmpty() || keyX != APP_DOT_NAME + key)
    {
        keyXSetting.setValue("Default", APP_DOT_NAME + key);
    }

    //
    const QString &keyString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key;
    QSettings keySetting(keyString, QSettings::NativeFormat);
    keySetting.setValue("Default", key + QObject::tr("File"));

    const QString &iconString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key + "\\DefaultIcon";
    QSettings iconSetting(iconString, QSettings::NativeFormat);
    iconSetting.setValue("Default", QString("%1,%2").arg(QApplication::applicationFilePath().replace(TTK_SEPARATOR, "\\")).arg(1));

    const QString &openString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key + "\\Shell\\Open";
    QSettings openSetting(openString, QSettings::NativeFormat);
    openSetting.setValue("Default", QObject::tr("Use TTKMusicPlayer play"));

    const QString &openComString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key + "\\Shell\\Open\\Command";
    QSettings openComSetting(openComString, QSettings::NativeFormat);
    openComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace(TTK_SEPARATOR, "\\"))
                                     + QString(" %1 ").arg(MUSIC_OUTSIDE_OPEN) + QString("\"%1\""));

    const QString &playListString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key + "\\Shell\\PlayList";
    QSettings playListSetting(playListString, QSettings::NativeFormat);
    playListSetting.setValue("Default", QObject::tr("Add to TTKMusicPlayer Playlist"));

    const QString &playListComString = QString("HKEY_CURRENT_USER\\Software\\Classes\\") + APP_DOT_NAME + key + "\\Shell\\PlayList\\Command";
    QSettings playListComSetting(playListComString, QSettings::NativeFormat);
    playListComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace(TTK_SEPARATOR, "\\"))
                                         + QString(" %1 ").arg(MUSIC_OUTSIDE_LIST) + QString("\"%1\""));
    //
    const QString &fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    fileExtsSetting.setValue("Progid", APP_DOT_NAME + key);

    const QString &fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    fileExtsUserSetting.setValue("Progid", APP_DOT_NAME + key);

}
#endif
