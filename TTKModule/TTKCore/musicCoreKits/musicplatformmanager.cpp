#include "musicplatformmanager.h"

#ifdef Q_OS_WIN
#include <QSettings>
#include <QApplication>
#include <qt_windows.h>

void MusicPlatformManager::windowsEnableLeftMode()
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

void MusicPlatformManager::windowsStartUpMode(bool v) const
{
    const QString &appName = TTK_APP_NAME;
    QString appPath = TTK_SERVICE_FULL;
    appPath.replace("/", "\\");

    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QSettings regWOW("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (v)
    {
        if (reg.value(appName).toString() != appPath)
        {
            reg.setValue(appName, QString("\"%1\"").arg(appPath));
        }

        if (regWOW.value(appName).toString() != appPath)
        {
            regWOW.setValue(appName, QString("\"%1\"").arg(appPath));
        }
    }
    else
    {
        if (reg.contains(appName))
        {
            reg.remove(appName);
            reg.sync();
        }

        if (regWOW.contains(appName))
        {
            regWOW.remove(appName);
            regWOW.sync();
        }
    }
}
#endif
