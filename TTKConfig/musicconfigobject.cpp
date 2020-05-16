#include "musicconfigobject.h"
#include "musicconfigdefine.h"

#include <QProcess>

MusicConfigObject::MusicConfigObject(QObject *parent)
    : QObject(parent)
{

}

void MusicConfigObject::checkValid()
{
    checkTheDirectoryExist();
    checkTheFileNeededExist();
}

QString MusicConfigObject::getAppPath() const
{
    return C_TTKSERVICE_FULL;
}

void MusicConfigObject::initialize()
{
    checkTheFileNeededExist();

    copyFileOverwrite(":/data/musicconfig.xml", C_COFIGPATH_FULL);
    copyFileOverwrite(":/data/music.tkpl", C_MUSICPATH_FULL);
    copyFileOverwrite(":/data/musicdown.ttk", C_NORMALDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloud.ttk", C_CLOUDDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloudp.ttk", C_CLOUDUPPATH_FULL);
    copyFileOverwrite(":/data/musichistory.ttk", C_MUSICSEARCH_FULL);
    copyFileOverwrite(":/data/musicuser.dll", C_DARABASEPATH_FULL);
    copyFileOverwrite(":/data/musicuser.ttk", C_USERPATH_FULL);
    copyFileOverwrite(":/data/musicbarrage.ttk", C_BARRAGEPATH_FULL);
}

void MusicConfigObject::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkpath(name);
    }
}

void MusicConfigObject::checkTheDirectoryExist()
{
    dirIsExist(C_LRC_DIR_FULL);
    dirIsExist(C_MUSIC_DIR_FULL);
    dirIsExist(C_MOVIE_DIR_FULL);
    dirIsExist(C_UPDATE_DIR_FULL);

    dirIsExist(C_CACHE_DIR_FULL);
    dirIsExist(C_ART_DIR_FULL);
    dirIsExist(C_BACKGROUND_DIR_FULL);
    dirIsExist(C_SCREEN_DIR_FULL);

    dirIsExist(C_AVATAR_DIR_FULL);
    dirIsExist(C_USER_THEME_DIR_FULL);

    dirIsExist(C_THEME_DIR_FULL);
    dirIsExist(C_PLUGINS_DIR_FULL);
    dirIsExist(C_LANGUAGE_DIR_FULL);
}

void MusicConfigObject::checkTheFileNeededExist()
{
    copyFile(":/data/musicconfig.xml", C_COFIGPATH_FULL);
    copyFile(":/data/music.tkpl", C_MUSICPATH_FULL);
    copyFile(":/data/musicdown.ttk", C_NORMALDOWNPATH_FULL);
    copyFile(":/data/musiccloud.ttk", C_CLOUDDOWNPATH_FULL);
    copyFile(":/data/musiccloudp.ttk", C_CLOUDUPPATH_FULL);
    copyFile(":/data/musichistory.ttk", C_MUSICSEARCH_FULL);
    copyFile(":/data/musicuser.dll", C_DARABASEPATH_FULL);
    copyFile(":/data/musicuser.ttk", C_USERPATH_FULL);
    copyFile(":/data/musicbarrage.ttk", C_BARRAGEPATH_FULL);

#ifdef Q_OS_UNIX
    copyLinuxShellFile(":/data/TTKRoutine.sh", C_TTKROUTINE_FULL);
    copyLinuxShellFile(":/data/TTKInit.sh", C_TTKINIT_FULL);
    copyLinuxShellFile(":/data/TTKMusicPlayer.sh", C_TTKMUSICPLAYER_FULL);
    copyLinuxShellFile(":/data/TTKConsole.sh", C_TTKCONSOLE_FULL);
    copyLinuxShellFile(":/data/TTKService.sh", C_TTKSERVICE_FULL);
    copyLinuxShellFile(":/data/TTKRoutineCopy.sh", C_TTKROUTINECOPY_FULL);
#endif
}

void MusicConfigObject::copyFileOverwrite(const QString &origin, const QString &des)
{
    if(QFile::exists(des))
    {
        QFile::remove(des);
    }
    QFile::copy(origin, des);
    QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
}

void MusicConfigObject::copyFile(const QString &origin, const QString &des)
{
    if(!QFile::exists(des))
    {
        QFile::copy(origin, des);
        QFile::setPermissions(des, QFile::ReadOwner | QFile::WriteOwner);
    }
}

void MusicConfigObject::copyLinuxShellFile(const QString &name, const QString &path)
{
    copyFileOverwrite(name, path);
    QProcess::execute("chmod", QStringList() << "+x" << path);
}
