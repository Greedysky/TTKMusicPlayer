/***************************************************************************
 *   Copyright (C) 2008-2014 by Ilya Kotov                                 *
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
#include "fileinfo.h"

FileInfo::FileInfo(const QString &path)
{
    m_path = path;
    m_length = 0;
    m_metaData.insert(Qmmp::URL, path);
}

FileInfo::FileInfo(const FileInfo &other)
{
    *this = other;
}

FileInfo::~FileInfo()
{}

void FileInfo::operator=(const FileInfo &info)
{
    setLength(info.length());
    setMetaData(info.metaData());
    setPath(info.path());
}

bool FileInfo::operator==(const FileInfo &info)
{
    return metaData () == info.metaData () &&
            length () == info.length () &&
            path() == info.path();
}

bool FileInfo::operator!=(const FileInfo &info)
{
    return !operator==(info);
}

qint64 FileInfo::length () const
{
    return m_length;
}

const QString FileInfo::metaData (Qmmp::MetaData key) const
{
    return m_metaData[key];
}

const QMap<Qmmp::MetaData, QString>  FileInfo::metaData () const
{
    return m_metaData;
}

void FileInfo::setMetaData(const QMap<Qmmp::MetaData,  QString> &metaData)
{
    m_metaData = metaData;
    if(m_metaData.value(Qmmp::URL).isEmpty())
         m_metaData.insert(Qmmp::URL, m_path);

    foreach(QString value, m_metaData.values()) //remove empty keys
    {
        if (value.isEmpty() || value == "0")
            m_metaData.remove(m_metaData.key(value));
    }
}

bool FileInfo::isEmpty() const
{
    return m_metaData.isEmpty(); //TODO add correct test
}

const QString FileInfo::path() const
{
    return m_path;
}

void FileInfo::setLength(qint64 length)
{
    m_length = length;
}

void FileInfo::setMetaData(Qmmp::MetaData key, const QString &value)
{
    if (value.isEmpty() || value == "0")
        return;

    //extract track number from "Track Number/Total Tracks Number" string
    if((key == Qmmp::TRACK || key == Qmmp::DISCNUMBER) && value.contains("/"))
        m_metaData.insert(key, value.section("/",0,0));
    else
        m_metaData.insert(key, value);
}

void FileInfo::setMetaData(Qmmp::MetaData key, int value)
{
    if(value != 0)
        m_metaData.insert(key, QString::number(value));
}

void FileInfo::setPath(const QString &path)
{
    m_path = path;
    m_metaData.insert(Qmmp::URL, path);
}
