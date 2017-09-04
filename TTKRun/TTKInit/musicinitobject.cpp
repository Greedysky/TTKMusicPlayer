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
    copyFileOverwrite(":/data/music.lis", S_MUSICPATH_FULL);
    copyFileOverwrite(":/data/musicdown.ttk", S_NORMALDOWNPATH_FULL);
    copyFileOverwrite(":/data/musicdown.ttk", S_CLOUDDOWNPATH_FULL);
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
    copyFile(":/data/music.lis", S_MUSICPATH_FULL);
    copyFile(":/data/musicdown.ttk", S_NORMALDOWNPATH_FULL);
    copyFile(":/data/musicdown.ttk", S_CLOUDDOWNPATH_FULL);
    copyFile(":/data/musichistory.ttk", S_MUSICSEARCH_FULL);
    copyFile(":/data/musicuser.dll", S_DARABASEPATH_FULL);
    copyFile(":/data/musicuser.ttk", S_USERPATH_FULL);
    copyFile(":/data/musicbarrage.ttk", S_BARRAGEPATH_FULL);

#ifdef Q_OS_UNIX
    if(!QFile::exists(S_TTKDD_FULL))
    {
        QFile::copy(":/data/TTKLDD.sh", S_TTKDD_FULL);
        QFile::setPermissions(S_TTKDD_FULL, QFile::ReadOwner | QFile::WriteOwner);
        QProcess::execute("chmod", QStringList() << "+x" << S_TTKDD_FULL);
    }
    if(!QFile::exists(S_TTKSERVICE_FULL))
    {
        QFile::copy(":/data/TTKService.sh", S_TTKSERVICE_FULL);
        QFile::setPermissions(S_TTKSERVICE_FULL, QFile::ReadOwner | QFile::WriteOwner);
        QProcess::execute("chmod", QStringList() << "+x" << S_TTKSERVICE_FULL);
    }
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
