#include "ttkmusicutils.h"
#include "musicobject.h"
#include "musictime.h"

#include <QDir>

TTKMusicUtils::TTKMusicUtils(QObject *parent)
    : QObject(parent)
{

}

QString TTKMusicUtils::getRoot() const
{
    return MusicObject::getAppDir();
}

QString TTKMusicUtils::getRootPath() const
{
    QString path = getRoot();
#if defined (Q_OS_ANDROID)
    path.remove("TTKMobile/");
#endif
    return "file://" + path;
}

QString TTKMusicUtils::normalizeTime(qint64 time, const QString &format)
{
    return MusicTime(time, MusicTime::All_Msec).toString(format);
}

void TTKMusicUtils::checkTheFileNeededExist()
{
    checkTheDirectoryExist();
    if(!QFile::exists(COFIGPATH_FULL))
    {
        QFile(":/extra/musicconfig").copy(COFIGPATH_FULL);
        QFile::setPermissions(COFIGPATH_FULL, QFile::WriteOwner | QFile::ReadOwner);
    }

    if(!QFile::exists(MUSICPATH_FULL))
    {
        QFile(":/extra/music").copy(MUSICPATH_FULL);
        QFile::setPermissions(MUSICPATH_FULL, QFile::WriteOwner | QFile::ReadOwner);
    }

    QFile(":/extra/TTKMobile").copy(LANGUAGE_DIR_FULL + "TTKMobile.ln");
    QFile::setPermissions(LANGUAGE_DIR_FULL + "TTKMobile.ln", QFile::WriteOwner | QFile::ReadOwner);
}

void TTKMusicUtils::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void TTKMusicUtils::checkTheDirectoryExist()
{
    dirIsExist(APPDATA_DIR_FULL);
    dirIsExist(DOWNLOADS_DIR_FULL);

    dirIsExist(LRC_DIR_FULL);
    dirIsExist(MUSIC_DIR_FULL);
    dirIsExist(MOVIE_DIR_FULL);
    dirIsExist(CACHE_DIR_FULL);
    dirIsExist(ART_DIR_FULL);
    dirIsExist(BACKGROUND_DIR_FULL);

    dirIsExist(LANGUAGE_DIR_FULL);
}
