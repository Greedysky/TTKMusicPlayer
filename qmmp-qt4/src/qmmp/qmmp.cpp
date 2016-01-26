/***************************************************************************
 *   Copyright (C) 2008-2013 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#include <QApplication>
#include <QSettings>
#include <QLocale>
#include <QByteArray>

#ifndef LIB_DIR
#define LIB_DIR "/lib"
#endif

#define DEV_SUFFIX "dev"

#include "qmmp.h"

QString Qmmp::m_configDir;
QString Qmmp::m_langID;

const QString Qmmp::configFile()
{
    return configDir() + "qmmprc";
}

const QString Qmmp::configDir()
{
    return m_configDir.isEmpty() ? QDir::homePath() +"/.qmmp/" : m_configDir;
}

void Qmmp::setConfigDir(const QString &path)
{
    m_configDir = path;
    if(!m_configDir.endsWith('/'))
        m_configDir.append('/');
}

const QString Qmmp::strVersion()
{
    QString ver = QString("%1.%2.%3")
            .arg(QMMP_VERSION_MAJOR)
            .arg(QMMP_VERSION_MINOR)
            .arg(QMMP_VERSION_PATCH);
#if !QMMP_VERSION_STABLE
#ifdef SVN_REVISION
    ver += "-svn-"SVN_REVISION;
#else
    ver += "-"DEV_SUFFIX;
#endif
#endif
    return ver;
}

const QString Qmmp::pluginsPath()
{
    QByteArray path = qgetenv("QMMP_PLUGINS");
    if (!path.isEmpty())
       return path;
#ifdef QMMP_INSTALL_PREFIX
    QDir dir(QMMP_INSTALL_PREFIX"/"LIB_DIR"/qmmp");
    //qDebug(QMMP_INSTALL_PREFIX"/"LIB_DIR"/qmmp");
#else
#if defined(Q_OS_WIN) && !defined(Q_OS_CYGWIN)
    QDir dir(qApp->applicationDirPath() + "/plugins");
#else
    QDir dir(qApp->applicationDirPath() + "/../"LIB_DIR"/qmmp");
#endif
#endif
    return dir.canonicalPath();
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
    if (v.isEmpty())
        v = qgetenv ("LC_MESSAGES");
    if (v.isEmpty())
        v = qgetenv ("LANG");
    if (!v.isEmpty())
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
