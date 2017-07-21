#include "musicregeditmanager.h"
#include "musicformats.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif
#include <QSettings>
#include <QStringList>
#include <QApplication>

QString MusicRegeditManager::getClassName()
{
    return "MusicRegeditManager";
}

bool MusicRegeditManager::isFileAssociate()
{
    return currentNodeHasExist("mp3");
}

void MusicRegeditManager::setMusicRegeditAssociateFileIcon()
{
    QStringList types = MusicFormats::supportFormatsString();
    for(int i=0; i<types.count(); ++i)
    {
        const QString type = types[i];
        if(!currentNodeHasExist(type))
        {
            createMusicRegedit(type);
        }
    }
}

void MusicRegeditManager::setDesktopWallAutoStart(bool state)
{
    const QString REG_RUN = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QString applicationName = QApplication::applicationName();
    QSettings settings(REG_RUN, QSettings::NativeFormat);
    state ? settings.setValue(applicationName, QApplication::applicationFilePath().replace("/", "\\"))
          : settings.remove(applicationName);
}

void MusicRegeditManager::getDesktopWallControlPanel(QString &originPath, int &originType)
{
    QSettings settings("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
    originPath = settings.value("Wallpaper").toString();
    originType = settings.value("WallpaperStyle").toInt();
}

void MusicRegeditManager::setDesktopWallControlPanel(const QString &originPath, int originType)
{
    QSettings settings("HKEY_CURRENT_USER\\Control Panel\\Desktop", QSettings::NativeFormat);
    settings.setValue ("Wallpaper", originPath);
    QString wallpaperStyle = (originType == 0 || originType == 1) ? "00" : "10";
    settings.setValue ("WallpaperStyle", wallpaperStyle);
}

void MusicRegeditManager::setLeftWinEnable()
{
#ifdef Q_OS_WIN
    INPUT input[4];
    memset(input, 0, sizeof(input));
    input[0].type = input[1].type = input[2].type = input[3].type = INPUT_KEYBOARD;
    input[0].ki.wVk = input[2].ki.wVk = VK_LWIN;
    input[1].ki.wVk = input[3].ki.wVk = 0x44;
    input[2].ki.dwFlags = input[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, input, sizeof(INPUT));
#endif
}

bool MusicRegeditManager::currentNodeHasExist(const QString &key)
{
    bool state = false;
    QString keyX = "HKEY_CURRENT_USER\\Software\\Classes\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    state = (keyXSetting.value("Default").toString() == "TTKMusicPlayer." + key);

    const QString fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    state &= (fileExtsSetting.value("Progid").toString() == "TTKMusicPlayer." + key);

    const QString fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    state &= (fileExtsUserSetting.value("Progid").toString() == "TTKMusicPlayer." + key);

    return state;
}

void MusicRegeditManager::createMusicRegedit(const QString &key)
{
    QString keyX = "HKEY_CURRENT_USER\\Software\\Classes\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    keyX = keyXSetting.value("Default").toString();
    if(keyX.isEmpty() || keyX != "TTKMusicPlayer." + key)
    {
        keyXSetting.setValue("Default", "TTKMusicPlayer." + key);
    }

    ////////////////////////////////////////////////////////
    const QString keyString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key;
    QSettings keySetting(keyString, QSettings::NativeFormat);
    keySetting.setValue("Default", key + QObject::tr("File"));

    const QString iconString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key + "\\DefaultIcon";
    QSettings iconSetting(iconString, QSettings::NativeFormat);
    iconSetting.setValue("Default", QString("%1,%2").arg(QApplication::applicationFilePath().replace("/", "\\")).arg(1));

    const QString openString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key + "\\Shell\\Open";
    QSettings openSetting(openString, QSettings::NativeFormat);
    openSetting.setValue("Default", QObject::tr("user TTKMusicPlayer play"));

    const QString openComString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key + "\\Shell\\Open\\Command";
    QSettings openComSetting(openComString, QSettings::NativeFormat);
    openComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                     + QString(" -Open \"%1\""));

    const QString playListString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key + "\\Shell\\PlayList";
    QSettings playListSetting(playListString, QSettings::NativeFormat);
    playListSetting.setValue("Default", QObject::tr("add TTKMusicPlayer playList"));

    const QString playListComString = "HKEY_CURRENT_USER\\Software\\Classes\\TTKMusicPlayer." + key + "\\Shell\\PlayList\\Command";
    QSettings playListComSetting(playListComString, QSettings::NativeFormat);
    playListComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                         + QString(" -List \"%1\""));

    ////////////////////////////////////////////////////////
    const QString fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    fileExtsSetting.setValue("Progid", "TTKMusicPlayer." + key);

    const QString fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    fileExtsUserSetting.setValue("Progid", "TTKMusicPlayer." + key);

}
