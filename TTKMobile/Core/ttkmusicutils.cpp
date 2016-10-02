#include "ttkmusicutils.h"
#include "musicobject.h"
#include "musictime.h"

#include <QDir>
#include <QApplication>
#if defined (Q_OS_ANDROID)
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif

TTKMusicUtils::TTKMusicUtils(QObject *parent)
    : QObject(parent)
{

}

QString TTKMusicUtils::getRoot() const
{
    return MusicObject::getAppDir();
}

QString TTKMusicUtils::normalizeTime(qint64 time, const QString &format)
{
    return MusicTime(time, MusicTime::All_Msec).toString(format);
}
