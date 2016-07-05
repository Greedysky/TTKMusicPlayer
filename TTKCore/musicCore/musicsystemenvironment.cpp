#include "musicsystemenvironment.h"

#include <QProcess>

QString MusicSystemEnvironment::getClassName()
{
    return "MusicSystemEnvironment";
}

QString MusicSystemEnvironment::getData(const QString &data)
{
    foreach(QString environment, QProcess::systemEnvironment())
    {
        if(environment.startsWith(data, Qt::CaseInsensitive))
        {
            return environment.mid(QString(data).length());
        }
    }
    return QString();
}

QString MusicSystemEnvironment::getSystemDrive()
{
    return getData("SYSTEMDRIVE=");
}

QString MusicSystemEnvironment::getSystemRoot()
{
    return getData("SYSTEMROOT=");
}

QString MusicSystemEnvironment::getProgramData()
{
    return getData("PROGRAMDATA=");
}

QString MusicSystemEnvironment::getWinDir()
{
    return getData("WINDIR=");
}

QString MusicSystemEnvironment::getUserName()
{
    return getData("USERNAME=");
}

QString MusicSystemEnvironment::getPublic()
{
    return getData("PUBLIC=");
}

QString MusicSystemEnvironment::getLocalAppData()
{
    return getData("LOCALAPPDATA=");
}

QString MusicSystemEnvironment::getAppData()
{
    return getData("APPDATA=");
}

QString MusicSystemEnvironment::getTMP()
{
    return getData("TMP=");
}

QString MusicSystemEnvironment::getTEMP()
{
    return getData("TEMP=");
}

QString MusicSystemEnvironment::getCommonProgramFiles()
{
    return getData("COMMONPROGRAMFILES=");
}

QString MusicSystemEnvironment::getCommonProgramW64()
{
    return getData("COMMONPROGRAMW6432=");
}

QString MusicSystemEnvironment::getOS()
{
    return getData("OS=");
}
