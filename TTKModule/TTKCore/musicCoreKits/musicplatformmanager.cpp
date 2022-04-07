#include "musicplatformmanager.h"
#include "musicformats.h"

#include <QScreen>
#include <QSettings>
#include <QProcess>
#include <QStringList>
#include <QApplication>

#ifdef Q_OS_WIN
#  include <qt_windows.h>
#  include <ole2.h>
#  include <shobjidl.h>
#  include <shlobj.h>
#elif defined Q_OS_UNIX
#  include <X11/Xlib.h>
#endif
#define DEFAULT_DPI 96

#ifdef Q_OS_WIN
bool MusicPlatformManager::isFileAssociate()
{
    return currentNodeExist(MP3_FILE_PREFIX);
}

void MusicPlatformManager::setMusicRegeditAssociateFileIcon()
{
    const QStringList &types = MusicFormats::supportMusicFormats();
    for(int i = 0; i < types.count(); ++i)
    {
        const QString &type = types[i];
        if(!currentNodeExist(type))
        {
            createMusicRegedit(type);
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

MusicPlatformManager::SystemType MusicPlatformManager::systemName() const
{
#ifdef Q_OS_WIN
    typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
    HINSTANCE instance = LoadLibraryW(L"ntdll.dll");
    DWORD major, minor, buildNumber;
    NTPROC proc = TTKVoid_cast(NTPROC)GetProcAddress(instance, "MiniDumpWriteDump");
    proc(&major, &minor, &buildNumber);

    if(major == 6 && minor == 3)	//win 8.1
    {
        return SystemWin81;
    }
    if(major == 10 && minor == 0)	//win 10
    {
        return SystemWin10;
    }

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
                            return SystemWinNT40;
                        }
                        else if(os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
                        {
                            return SystemWin95;
                        }
                        break;
                    case 10: return SystemWin98;
                    case 90: return SystemWinMe;
                    default: break;
                }
                break;
            case 5:
                switch(os.dwMinorVersion)
                {
                    case 0: return SystemWin2000;
                    case 1: return SystemWinXP;
                    case 2:
                        if(os.wProductType == VER_NT_WORKSTATION && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                        {
                            return SystemWinXPProfessionalEdition;
                        }
                        else if(GetSystemMetrics(SM_SERVERR2) == 0)
                        {
                            return SystemWinServer2003;
                        }
                        else if(GetSystemMetrics(SM_SERVERR2) != 0)
                        {
                            return SystemWinServer2003R2;
                        }
                    default: break;
                }
                break;
            case 6:
                switch(os.dwMinorVersion)
                {
                    case 0: return os.wProductType == VER_NT_WORKSTATION ? SystemWinVista : SystemWinServer2008;
                    case 1: return os.wProductType == VER_NT_WORKSTATION ? SystemWin7 : SystemWinServer2008R2;
                    case 2: return os.wProductType == VER_NT_WORKSTATION ? SystemWin8 : SystemWinServer2012;
                    default: break;
                }
                break;
            default: return SystemUnkown;
        }
    }
    FreeLibrary(instance);
#elif defined Q_OS_LINUX
    QFile lsb("/etc/lsb-release");
    if(lsb.open(QFile::ReadOnly))
    {
        QRegExp regx("DISTRIB_ID=(\\w+)");
        if(QString(lsb.readAll()).indexOf(regx) != -1)
        {
            const QString &system = regx.cap(1).toLower();
            if(system == "ubuntu")
            {
                return SystemLinuxUbuntu;
            }
            else if(system == "debian")
            {
                return SystemLinuxDebian;
            }
            else if(system == "arch")
            {
                return SystemLinuxArch;
            }
            else if(system == "centos")
            {
                return SystemLinuxCentOS;
            }
        }
    }
    return SystemLinux;
#else
    return SystemMac;
#endif
    return SystemUnkown;
}

void MusicPlatformManager::setFileLink(const QString &src, const QString &des, const QString &ico, const QString &args, const QString &description)
{
#ifdef Q_OS_WIN
    HRESULT hres = CoInitialize(nullptr);
    if(SUCCEEDED(hres))
    {
        IShellLinkW *psl = nullptr;
        hres = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);

        if(SUCCEEDED(hres))
        {
            IPersistFile *ppf = nullptr;
            if(!src.isEmpty())
            {
                psl->SetPath(src.toStdWString().c_str());
            }

            if(!ico.isEmpty())
            {
                psl->SetIconLocation(ico.toStdWString().c_str(), 0);
            }

            if(!args.isEmpty())
            {
                psl->SetArguments(args.toStdWString().c_str());
            }

            if(!description.isEmpty())
            {
                psl->SetDescription(description.toStdWString().c_str());
            }

            hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
            if(SUCCEEDED(hres))
            {
                ppf->Save(des.toStdWString().c_str(), FALSE);
                ppf->Release();
            }
            psl->Release();
        }
    }
#else
    Q_UNUSED(src);
    Q_UNUSED(des);
    Q_UNUSED(ico);
    Q_UNUSED(args);
    Q_UNUSED(description);
#endif
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
