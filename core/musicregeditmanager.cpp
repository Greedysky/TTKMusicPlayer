#include "musicregeditmanager.h"

#include <QSettings>
#include <QApplication>

MusicRegeditManager::MusicRegeditManager()
{

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

void MusicRegeditManager::setMusicRegeditAssociateFileIcon()
{
    createMusicRegedit("aac");
    createMusicRegedit("ac3");
    createMusicRegedit("flac");
    createMusicRegedit("mp1");
    createMusicRegedit("mp2");
    createMusicRegedit("mp3");
    createMusicRegedit("oga");
    createMusicRegedit("ogg");
    createMusicRegedit("pmc");
    createMusicRegedit("wav");
}

void MusicRegeditManager::createMusicRegedit(const QString &key)
{
//    QString keyX = "HKEY_CLASSES_ROOT\\." + key;
//    QSettings keyXSetting(keyX, QSettings::NativeFormat);
//    keyX = keyXSetting->value("Default").toString();
//    if(keyX.isEmpty() ||
//       keyX != "QMusicPlayer." + key)
//    {
//        keyXSetting->setValue("Default", "QMusicPlayer." + key);
//    }

    const QString keyString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key;
    QSettings keySetting(keyString, QSettings::NativeFormat);
    keySetting.setValue("Default", key + QObject::tr("File"));

    const QString iconString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\DefaultIcon";
    QSettings iconSetting(iconString, QSettings::NativeFormat);
    iconSetting.setValue("Default", QApplication::applicationFilePath().replace("/", "\\"));

    const QString openString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\Open";
    QSettings openSetting(openString, QSettings::NativeFormat);
    openSetting.setValue("Default", "user QMusicPlayer play");

    const QString openComString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\Open\\Command";
    QSettings openComSetting(openComString, QSettings::NativeFormat);
    openComSetting.setValue("Default", QApplication::applicationFilePath().replace("/", "\\"));

    const QString playListString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\PlayList";
    QSettings playListSetting(playListString, QSettings::NativeFormat);
    playListSetting.setValue("Default", "add QMusicPlayer playList");

    const QString playListComString = "HKEY_CLASSES_ROOT\\QMusicPlayer." + key + "\\Shell\\PlayList\\Command";
    QSettings playListComSetting(playListComString, QSettings::NativeFormat);
    playListComSetting.setValue("Default", QApplication::applicationFilePath().replace("/", "\\"));
}
