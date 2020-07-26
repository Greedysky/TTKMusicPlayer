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
    return TTK_SERVICE_FULL;
}

void MusicConfigObject::initialize()
{
    checkTheFileNeededExist();

    copyFileOverwrite(":/data/musicconfig.xml", TTK_COFIGPATH_FULL);
    copyFileOverwrite(":/data/music.tkpl", TTK_MUSICPATH_FULL);
    copyFileOverwrite(":/data/musicdown.ttk", TTK_NORMALDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloud.ttk", TTK_CLOUDDOWNPATH_FULL);
    copyFileOverwrite(":/data/musiccloudp.ttk", TTK_CLOUDUPPATH_FULL);
    copyFileOverwrite(":/data/musichistory.ttk", TTK_MUSICSEARCH_FULL);
    copyFileOverwrite(":/data/musicuser.dll", TTK_DARABASEPATH_FULL);
    copyFileOverwrite(":/data/musicuser.ttk", TTK_USERPATH_FULL);
    copyFileOverwrite(":/data/musicbarrage.ttk", TTK_BARRAGEPATH_FULL);
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
    dirIsExist(TTK_LRC_DIR_FULL);
    dirIsExist(TTK_MUSIC_DIR_FULL);
    dirIsExist(TTK_MOVIE_DIR_FULL);
    dirIsExist(TTK_UPDATE_DIR_FULL);

    dirIsExist(TTK_CACHE_DIR_FULL);
    dirIsExist(TTK_ART_DIR_FULL);
    dirIsExist(TTK_BACKGROUND_DIR_FULL);
    dirIsExist(TTK_SCREEN_DIR_FULL);

    dirIsExist(TTK_AVATAR_DIR_FULL);
    dirIsExist(TTK_USER_THEME_DIR_FULL);

    dirIsExist(TTK_THEME_DIR_FULL);
    dirIsExist(TTK_PLUGINS_DIR_FULL);
    dirIsExist(TTK_LANGUAGE_DIR_FULL);
}

void MusicConfigObject::checkTheFileNeededExist()
{
    copyFile(":/data/musicconfig.xml", TTK_COFIGPATH_FULL);
    copyFile(":/data/music.tkpl", TTK_MUSICPATH_FULL);
    copyFile(":/data/musicdown.ttk", TTK_NORMALDOWNPATH_FULL);
    copyFile(":/data/musiccloud.ttk", TTK_CLOUDDOWNPATH_FULL);
    copyFile(":/data/musiccloudp.ttk", TTK_CLOUDUPPATH_FULL);
    copyFile(":/data/musichistory.ttk", TTK_MUSICSEARCH_FULL);
    copyFile(":/data/musicuser.dll", TTK_DARABASEPATH_FULL);
    copyFile(":/data/musicuser.ttk", TTK_USERPATH_FULL);
    copyFile(":/data/musicbarrage.ttk", TTK_BARRAGEPATH_FULL);

#ifdef Q_OS_UNIX
    copyLinuxShellFile(":/data/TTKRoutine.sh", TTK_ROUTINE_FULL);
    copyLinuxShellFile(":/data/TTKInit.sh", TTK_INIT_FULL);
    copyLinuxShellFile(":/data/TTKMusicPlayer.sh", TTK_MUSICPLAYER_FULL);
    copyLinuxShellFile(":/data/TTKConsole.sh", TTK_CONSOLE_FULL);
    copyLinuxShellFile(":/data/TTKService.sh", TTK_SERVICE_FULL);
    copyLinuxShellFile(":/data/TTKRoutineCopy.sh", TTK_ROUTINECOPY_FULL);
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
