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
