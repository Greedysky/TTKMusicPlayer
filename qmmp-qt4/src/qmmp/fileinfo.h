/***************************************************************************
 *   Copyright (C) 2008-2009 by Ilya Kotov                                 *
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
#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>
#include <QMap>
#include <QUrl>

#include "qmmp.h"

/*! @brief The FileInfo class stores metadata and audio information about media file or stream.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class FileInfo
{
public:
    /*!
     * Constructs a new empty FileInfo that with file \b path
     */
    FileInfo(const QString &path = QString());
    /*!
     * Constructs a new FileInfo that is a copy of the given \b info
     */
    FileInfo(const FileInfo &info);
    /*!
     * Destructor.
     */
    ~FileInfo();
    /*!
     * Makes a copy of the given \b info
     */
    void operator=(const FileInfo &info);
    /*!
     * Returns \b true if this FileInfo object refers to \b info; otherwise returns \b false.
     */
    bool operator==(const FileInfo &info);
    /*!
     * Returns \b false if this FileInfo object refers to \b info; otherwise returns \b true.
     */
    bool operator!=(const FileInfo &info);
    /*!
     * Returnds file duration in seconds.
     */
    qint64 length () const;
    /*!
     * Returns the metdata string associated with the given \b key.
     */
    const QString metaData (Qmmp::MetaData key) const;
    /*!
     * Returns all meta data in map.
     */
    const QMap<Qmmp::MetaData, QString>  metaData () const;
    /*!
     * Returns \b true if stream/file has no metadata, otherwise returns \b false
     */
    bool isEmpty() const;
    /*!
     * Returns file path or stream url.
     */
    const QString path() const;
    /*!
     * Sets file duration to \b length in seconds.
     */
    void setLength(qint64 length);
    /*!
     * Sets metadata \b key to \b value
     */
    void setMetaData(Qmmp::MetaData key, const QString &value);
    /*!
     * Sets metadata \b key to \b value (integer is converted to string)
     */
    void setMetaData(Qmmp::MetaData key, int value);
    /*!
     * Changes all metadata to \b metaData
     */
    void setMetaData(const QMap <Qmmp::MetaData,  QString> &metaData);
    /*!
     * Sets file path or stream url to \b path
     */
    void setPath(const QString &path);

private:
    QMap <Qmmp::MetaData, QString> m_metaData;
    qint64 m_length;
    QString m_path;
    QList<QMap<int, int> > map;
};

#endif
