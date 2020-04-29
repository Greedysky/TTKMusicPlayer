/***************************************************************************
 *   Copyright (C) 2008-2020 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QLocale>
#include <QFile>
#include <QByteArray>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define DEV_SUFFIX "dev"

#include "qmmp.h"

QString Qmmp::m_configDir;
QString Qmmp::m_langID;
#ifdef Q_OS_WIN
QString Qmmp::m_appDir;
#endif

QString Qmmp::configFile()
{
    return configDir() + "/qmmprc";
}

QString Qmmp::configDir()
{
#ifdef Q_OS_WIN
    if(m_configDir.isEmpty())
    {
        if(isPortable())
            return m_appDir + "/.qmmp";
        else
            return  QDir::homePath() +"/.qmmp";
    }
    else
        return m_configDir;
#else
    return m_configDir.isEmpty() ? QDir::homePath() +"/.qmmp" : m_configDir;
#endif
}

void Qmmp::setConfigDir(const QString &path)
{
    m_configDir = path;
}

QString Qmmp::strVersion()
{
    QString ver = QString("%1.%2.%3")
            .arg(QMMP_VERSION_MAJOR)
            .arg(QMMP_VERSION_MINOR)
            .arg(QMMP_VERSION_PATCH);
#if !QMMP_VERSION_STABLE
#ifdef SVN_REVISION
    ver += "-svn-" SVN_REVISION;
#else
    ver += "-" DEV_SUFFIX;
#endif
#endif
    return ver;
}

QString Qmmp::pluginPath()
{
    QByteArray path = qgetenv("QMMP_PLUGINS");
    if(!path.isEmpty())
        return path;
		
    QDir dir(qApp->applicationDirPath() + "/plugins");
    return dir.canonicalPath();
}

QStringList Qmmp::findPlugins(const QString &prefix)
{
    QDir pluginDir(pluginPath() + "/" + prefix);
    QStringList paths;
    for(const QFileInfo &info : pluginDir.entryInfoList(QStringList() << "*.dll" << "*.so", QDir::Files))
        paths << info.canonicalFilePath();
    return paths;
}

QString Qmmp::systemLanguageID()
{
    if(m_langID.isEmpty())
    {
        m_langID = uiLanguageID();
        //qDebug("Qmmp: setting ui language to '%s'", qPrintable(m_langID));
    }

    if(m_langID != "auto")
        return m_langID;

#ifdef Q_OS_UNIX
    QByteArray v = qgetenv ("LC_ALL");
    if(v.isEmpty())
        v = qgetenv ("LC_MESSAGES");
    if(v.isEmpty())
        v = qgetenv ("LANG");
    if(!v.isEmpty())
        return QLocale (v).name();
#endif
    return  QLocale::system().name();
}

QString Qmmp::uiLanguageID()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString langID = settings.value("General/locale", "auto").toString();
    langID = langID.isEmpty() ? "auto" : langID;
    return langID;
}

void Qmmp::setUiLanguageID(const QString &code)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("General/locale", code);
    m_langID.clear();
}

QString Qmmp::dataPath()
{
//#if defined(Q_OS_WIN) && !defined(Q_OS_CYGWIN)
    return qApp->applicationDirPath();
//#else
//    return QDir(qApp->applicationDirPath() + "/../share/qmmp" APP_NAME_SUFFIX).absolutePath();
//#endif
}

#ifdef Q_OS_WIN
bool Qmmp::isPortable()
{
    if(m_appDir.isEmpty())
        m_appDir = QCoreApplication::applicationDirPath();
    return QFile::exists(m_appDir + "/qmmp_portable.txt");
}
#endif
