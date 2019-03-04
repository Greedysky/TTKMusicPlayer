#include "musicinitobject.h"

#include <QProcess>

MusicInitObject::MusicInitObject(QObject *parent)
    : QObject(parent)
{

}

void MusicInitObject::checkValid()
{
    checkTheDirectoryExist();
    checkTheFileNeededExist();
}

void MusicInitObject::init()
{
    checkTheFileNeededExist();

    copyFileOverwrite(":/data/musicconfig.xml", S_COFIGPATH_FULL);
    copyFileOverwrite(":/data/music.tkpl", S_MUSICPATH_FULL);
    copyFileOverwrite(":/data/musicdown.ttk", S_NORMALDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloud.ttk", S_CLOUDDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloudp.ttk", S_CLOUDUPPATH_FULL);
    copyFileOverwrite(":/data/musichistory.ttk", S_MUSICSEARCH_FULL);
    copyFileOverwrite(":/data/musicuser.dll", S_DARABASEPATH_FULL);
    copyFileOverwrite(":/data/musicuser.ttk", S_USERPATH_FULL);
    copyFileOverwrite(":/data/musicbarrage.ttk", S_BARRAGEPATH_FULL);
}

void MusicInitObject::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkpath(name);
    }
}

void MusicInitObject::checkTheDirectoryExist()
{
    dirIsExist(S_APPDATA_DIR_FULL);
    dirIsExist(S_DOWNLOADS_DIR_FULL);

    dirIsExist(S_LRC_DIR_FULL);
    dirIsExist(S_MUSIC_DIR_FULL);
    dirIsExist(S_MOVIE_DIR_FULL);
    dirIsExist(S_CACHE_DIR_FULL);
    dirIsExist(S_ART_DIR_FULL);
    dirIsExist(S_BACKGROUND_DIR_FULL);
    dirIsExist(S_UPDATE_DIR_FULL);

    dirIsExist(S_AVATAR_DIR_FULL);
    dirIsExist(S_USER_THEME_DIR_FULL);

    dirIsExist(S_THEME_DIR_FULL);
    dirIsExist(S_PLUGINS_DIR_FULL);
    dirIsExist(S_LANGUAGE_DIR_FULL);
}

void MusicInitObject::checkTheFileNeededExist()
{
    copyFile(":/data/musicconfig.xml", S_COFIGPATH_FULL);
    copyFile(":/data/music.tkpl", S_MUSICPATH_FULL);
    copyFile(":/data/musicdown.ttk", S_NORMALDOWNPATH_FULL);
    copyFile(":/data/musiccloud.ttk", S_CLOUDDOWNPATH_FULL);
    copyFile(":/data/musiccloudp.ttk", S_CLOUDUPPATH_FULL);
    copyFile(":/data/musichistory.ttk", S_MUSICSEARCH_FULL);
    copyFile(":/data/musicuser.dll", S_DARABASEPATH_FULL);
    copyFile(":/data/musicuser.ttk", S_USERPATH_FULL);
    copyFile(":/data/musicbarrage.ttk", S_BARRAGEPATH_FULL);

#ifdef Q_OS_UNIX
    copyLinuxShellFile(":/data/TTKInit.sh", S_TTKINIT_FULL);
    copyLinuxShellFile(":/data/TTKMusicPlayer.sh", S_TTKMUSICPLAYER_FULL);
    copyLinuxShellFile(":/data/TTKConsole.sh", S_TTKCONSOLE_FULL);
    copyLinuxShellFile(":/data/TTKService.sh", S_TTKSERVICE_FULL);
    copyLinuxShellFile(":/data/TTKRoutine.sh", S_TTKROUTINE_FULL);
    copyLinuxShellFile(":/data/TTKRoutineCopy.sh", S_TTKROUTINECOPY_FULL);
#endif
}

void MusicInitObject::copyFileOverwrite(const QString &origin, const QString &des)
{
    if(QFile::exists(des))
    {
        QFile::remove(des);
    }
    QFile::copy(origin, des);
    QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
}

void MusicInitObject::copyFile(const QString &origin, const QString &des)
{
    if(!QFile::exists(des))
    {
        QFile::copy(origin, des);
        QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
    }
}

void MusicInitObject::copyLinuxShellFile(const QString &name, const QString &path)
{
    copyFileOverwrite(name, path);
    QProcess::execute("chmod", QStringList() << "+x" << path);
}
