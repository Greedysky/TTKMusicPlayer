#include "musicregeditmanager.h"

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
    QStringList types;
    types << "pmc" << "wav" << "aac" << "ac3" << "flac" << "mp1"
          << "mp2" << "mp3" << "oga" << "ogg";
    for(int i=0; i<types.count(); ++i)
    {
        if(!currentNodeHasExsit(types[i]))
        {
            createMusicRegedit(types[i], i + 1);
        }
    }
}

bool MusicRegeditManager::currentNodeHasExsit(const QString &key)
{
    bool state = false;
    QString keyX = "HKEY_CLASSES_ROOT\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    state = (keyXSetting.value("Default").toString() == "QMusicPlayer." + key);

    const QString fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    state &= (fileExtsSetting.value("Progid").toString() == "QMusicPlayer." + key);

    const QString fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    state &= (fileExtsUserSetting.value("Progid").toString() == "QMusicPlayer." + key);

    return state;
}

void MusicRegeditManager::createMusicRegedit(const QString &key, int index)
{
    QString keyX = "HKEY_CLASSES_ROOT\\." + key;
    QSettings keyXSetting(keyX, QSettings::NativeFormat);
    keyX = keyXSetting.value("Default").toString();
    if(keyX.isEmpty() || keyX != "QMusicPlayer." + key)
    {
        keyXSetting.setValue("Default", "QMusicPlayer." + key);
    }

    ////////////////////////////////////////////////////////
    const QString keyString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key;
    QSettings keySetting(keyString, QSettings::NativeFormat);
    keySetting.setValue("Default", key + QObject::tr("File"));

    const QString iconString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\DefaultIcon";
    QSettings iconSetting(iconString, QSettings::NativeFormat);
    iconSetting.setValue("Default", QString("%1,%2").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                                    .arg(index));

    const QString openString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\Open";
    QSettings openSetting(openString, QSettings::NativeFormat);
    openSetting.setValue("Default", QObject::tr("user QMusicPlayer play"));

    const QString openComString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\Open\\Command";
    QSettings openComSetting(openComString, QSettings::NativeFormat);
    openComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                     + QString(" -Open \"%1\""));

    const QString playListString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\PlayList";
    QSettings playListSetting(playListString, QSettings::NativeFormat);
    playListSetting.setValue("Default", QObject::tr("add QMusicPlayer playList"));

    const QString playListComString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\PlayList\\Command";
    QSettings playListComSetting(playListComString, QSettings::NativeFormat);
    playListComSetting.setValue("Default", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\"))
                                         + QString(" -List \"%1\""));

    ////////////////////////////////////////////////////////
    const QString fileExtsString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key;
    QSettings fileExtsSetting(fileExtsString, QSettings::NativeFormat);
    fileExtsSetting.setValue("Progid", "QMusicPlayer." + key);

    const QString fileExtsUserString = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + key + "\\UserChoice";
    QSettings fileExtsUserSetting(fileExtsUserString, QSettings::NativeFormat);
    fileExtsUserSetting.setValue("Progid", "QMusicPlayer." + key);

}
