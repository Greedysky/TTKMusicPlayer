/***************************************************************************
 *   Copyright (C) 2008 by Ilya Kotov                                      *
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

#include <QProcess>

#include "archivereader.h"

ArchiveReader::ArchiveReader(QObject *parent)
        : QObject(parent)
{
    m_process = new QProcess(this);
}


ArchiveReader::~ArchiveReader()
{}

bool ArchiveReader::isSupported(const QString &path)
{
    QString lPath = path.toLower();
    return lPath.endsWith(".mdz") ||
           lPath.endsWith(".s3z") ||
           lPath.endsWith(".xmz") ||
           lPath.endsWith(".itz") ||
           lPath.endsWith(".mdgz") ||
           lPath.endsWith(".s3gz") ||
           lPath.endsWith(".xmgz") ||
           lPath.endsWith(".itgz") ||
           lPath.endsWith(".mdbz") ||
           lPath.endsWith(".s3bz") ||
           lPath.endsWith(".xmbz") ||
           lPath.endsWith(".itbz");
}

QByteArray ArchiveReader::unpack(const QString &path)
{
    QString lPath = path.toLower();
    if (path.endsWith(".mdz") ||
            lPath.endsWith(".s3z") ||
            lPath.endsWith(".xmz") ||
            lPath.endsWith(".itz"))
        return unzip(path);
    else if (lPath.endsWith(".mdgz") ||
             lPath.endsWith(".s3gz") ||
             lPath.endsWith(".xmgz") ||
             lPath.endsWith(".itgz"))
        return gunzip(path);
    else if (lPath.endsWith(".mdbz"))
        return bunzip2(path);

    return QByteArray();
}

QByteArray ArchiveReader::unzip(const QString &path)
{
    QStringList args;
    args << "-p" << path;
    m_process->start("unzip", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput ();
}

QByteArray ArchiveReader::gunzip(const QString &path)
{
    QStringList args;
    args << "-c" << path;
    m_process->start("gunzip", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput ();
}

QByteArray ArchiveReader::bunzip2(const QString &path)
{
    QStringList args;
    args << "-c" << path;
    m_process->start("bunzip2", args);
    m_process->waitForFinished();
    return m_process->readAllStandardOutput ();
}
