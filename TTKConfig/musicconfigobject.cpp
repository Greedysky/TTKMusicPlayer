#include "musicconfigobject.h"

#include <QProcess>

MusicConfigObject::MusicConfigObject(QObject *parent)
    : QObject(parent)
{

}

void MusicConfigObject::valid() const
{
    checkDirectoryExist();
    checkFileNeededExist();
}

void MusicConfigObject::initialize() const
{
    valid();

    copyFileOverwrite(":/data/config.xml", TTK_COFIG_PATH_FULL);
    copyFileOverwrite(":/data/playlist.tkpl", TTK_PLAYLIST_PATH_FULL);
    copyFileOverwrite(":/data/download.tkf", TTK_NORMAL_DOWN_PATH_FULL);
    copyFileOverwrite(":/data/cdownload.tkf", TTK_CLOUD_DOWN_PATH_FULL);
    copyFileOverwrite(":/data/cupload.tkf", TTK_CLOUD_UP_PATH_FULL);
    copyFileOverwrite(":/data/search.tkf", TTK_SEARCH_PATH_FULL);
}

void MusicConfigObject::reset() const
{
    copyFileOverwrite(":/data/config.xml", TTK_COFIG_PATH_FULL);
}

void MusicConfigObject::directoryExist(const QString &name) const
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkpath(name);
    }
}

void MusicConfigObject::checkDirectoryExist() const
{
    directoryExist(TTK_LRC_DIR_FULL);
    directoryExist(TTK_MUSIC_DIR_FULL);

    directoryExist(TTK_CACHE_DIR_FULL);
    directoryExist(TTK_ART_DIR_FULL);
    directoryExist(TTK_BACKGROUND_DIR_FULL);

    directoryExist(TTK_USER_THEME_DIR_FULL);

    directoryExist(TTK_THEME_DIR_FULL);
    directoryExist(TTK_PLUGINS_DIR_FULL);
    directoryExist(TTK_LANGUAGE_DIR_FULL);
}

void MusicConfigObject::checkFileNeededExist() const
{
    copyFile(":/data/config.xml", TTK_COFIG_PATH_FULL);
    copyFile(":/data/playlist.tkpl", TTK_PLAYLIST_PATH_FULL);
    copyFile(":/data/download.tkf", TTK_NORMAL_DOWN_PATH_FULL);
    copyFile(":/data/cdownload.tkf", TTK_CLOUD_DOWN_PATH_FULL);
    copyFile(":/data/cupload.tkf", TTK_CLOUD_UP_PATH_FULL);
    copyFile(":/data/search.tkf", TTK_SEARCH_PATH_FULL);

    copyFile(":/data/playlist", TTK_RESOURCE_DIR_FULL + "playlist");
    copyFile(":/data/toplist", TTK_RESOURCE_DIR_FULL + "toplist");
    copyFile(":/data/artistlist", TTK_RESOURCE_DIR_FULL + "artistlist");
    copyFile(":/data/movielist", TTK_RESOURCE_DIR_FULL + "movielist");
    copyFile(":/data/fmlist", TTK_RESOURCE_DIR_FULL + "fmlist");

#ifdef Q_OS_UNIX
    if(!QFile::exists(MAIN_DIR_FULL + "ttk_runtime"))
    {
        copyLinuxShellFile(":/data/TTKRoutine.sh", TTK_ROUTINE_FULL);
        copyLinuxShellFile(":/data/TTKMusicPlayer.sh", TTK_APP_MAIN_FULL);
        copyLinuxShellFile(":/data/TTKRoutineCopy.sh", TTK_ROUTINECOPY_FULL);
        copyLinuxShellFile(":/data/TTKInit.sh", TTK_INIT_FULL);
        copyLinuxShellFile(":/data/TTKConsole.sh", TTK_CONSOLE_FULL);
        copyLinuxShellFile(":/data/TTKService.sh", TTK_SERVICE_FULL);
    }
#endif
}

void MusicConfigObject::copyFileOverwrite(const QString &origin, const QString &des) const
{
    if(QFile::exists(des))
    {
        QFile::remove(des);
    }

    QFile::copy(origin, des);
    QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
}

void MusicConfigObject::copyFile(const QString &origin, const QString &des) const
{
    if(!QFile::exists(des))
    {
        QFile::copy(origin, des);
        QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
    }
}

#ifdef Q_OS_UNIX
void MusicConfigObject::copyLinuxShellFile(const QString &name, const QString &path) const
{
    copyFileOverwrite(name, path);
    QProcess::execute("chmod", {"+x", path});
}
#endif
