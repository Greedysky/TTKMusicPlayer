#include "ttkmusicutils.h"
#include "musictime.h"

TTKMusicUtils::TTKMusicUtils(QObject *parent)
    : QObject(parent)
{

}

QString TTKMusicUtils::normalizeTime(qint64 time, const QString &format)
{
    return MusicTime(time, MusicTime::All_Msec).toString(format);
}
