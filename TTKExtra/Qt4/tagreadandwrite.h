/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef TAGREADANDWRITE_H
#define TAGREADANDWRITE_H

#include <QMap>
#include <QString>
#include "qmmp_export.h"

/*! @brief The class of the file tag read and write.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT TagReadAndWrite
{
public:
    enum MusicTag
    {
        TAG_UNKNOWN = -1,
        TAG_TITLE = 0,   /*!< Title */
        TAG_ARTIST,      /*!< Artist  */
        TAG_ALBUMARTIST, /*!< Album artist  */
        TAG_ALBUM,       /*!< Album */
        TAG_COMMENT,     /*!< Comment */
        TAG_GENRE,       /*!< Genre */
        TAG_COMPOSER,    /*!< Composer */
        TAG_YEAR,        /*!< Year */
        TAG_TRACK,       /*!< Track number */
        TAG_DISCNUMBER,  /*!< Disc number */
        TAG_URL,         /*!< Stream url or local file path */
        TAG_LENGTH,      /*!< Length */
        TAG_FORMAT,      /*!< Format */
        TAG_SAMPLERATE,  /*!< Sample rate */
        TAG_MODE,        /*!< Mode */
        TAG_BITRATE,     /*!< Bitrate */
        TAG_CHANNEL,     /*!< Channel */
        TAG_COVER,       /*!< Cover */
    };

    TagReadAndWrite();
    TagReadAndWrite(const QString &file);
    ~TagReadAndWrite();

    bool readFile();
    bool readFile(const QString &path);

    bool writeMusicTag(MusicTag tag, const QString &value, int id3v2Version);
    inline QMap<MusicTag, QString> getMusicTags() const { return m_parameters; }

protected:
    QString m_path;
    QMap<MusicTag, QString> m_parameters;

};

#endif // TAGREADANDWRITE_H
