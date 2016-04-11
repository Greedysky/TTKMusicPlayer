#include "musicregeditmanager.h"
#include "musicplayer.h"

#include <QSettings>
#include <QStringList>
#include <QApplication>

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

void MusicRegeditManager::setMusicRegeditAssociateFileIcon()
{
    QStringList types = MusicPlayer::supportFormatsString();
    for(int i=0; i<types.count(); ++i)
    {
        if(!currentNodeHasExist(types[i]))
        {
            createMusicRegedit(types[i], i + 1);
        }
    }
}

bool MusicRegeditManager::currentNodeHasExist(const QString &key)
{
    bool state = false;
    QString keyX = "HKEY_CLASSES_ROOT\\." + key;
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

void MusicRegeditManager::createMusicRegedit(const QString &key, int index)
{
    QString keyX = "HKEY_CLASSES_ROOT\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    keyX = keyXSetting.value("Default").toString();
    if(keyX.isEmpty() || keyX != "TTKMusicPlayer." + key)
    {
        keyXSetting.setValue("Default", "TTKMusicPlayer." + key);
    }

    ////////////////////////////////////////////////////////
    const QString keyString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key;
    QSettings keySetting(keyString, QSettings::NativeFormat);
    keySetting.setValue("Default", key + QObject::tr("File"));

    const QString iconString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key + "\\DefaultIcon";
    QSettings iconSetting(iconString, QSettings::NativeFormat);
    iconSetting.setValue("Default", QString("%1,%2").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                                    .arg(index));

    const QString openString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key + "\\Shell\\Open";
    QSettings openSetting(openString, QSettings::NativeFormat);
    openSetting.setValue("Default", QObject::tr("user TTKMusicPlayer play"));

    const QString openComString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key + "\\Shell\\Open\\Command";
    QSettings openComSetting(openComString, QSettings::NativeFormat);
    openComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                     + QString(" -Open \"%1\""));

    const QString playListString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key + "\\Shell\\PlayList";
    QSettings playListSetting(playListString, QSettings::NativeFormat);
    playListSetting.setValue("Default", QObject::tr("add TTKMusicPlayer playList"));

    const QString playListComString = "HKEY_CLASSES_ROOT\\TTKMusicPlayer." + key + "\\Shell\\PlayList\\Command";
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
