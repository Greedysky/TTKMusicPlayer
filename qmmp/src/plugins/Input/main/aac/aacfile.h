/***************************************************************************
 *   Copyright (C) 2008-2024 by Ilya Kotov                                 *
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

#ifndef AACFILE_H
#define AACFILE_H

#include <QMap>

#include <taglib/id3v2tag.h>
#include <qmmp/qmmp.h>

class QIODevice;
class TagExtractor;
class QBuffer;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class AACFile
{
public:
    explicit AACFile(QIODevice *input, bool metaData = true, bool adts = true);

    qint64 duration() const;
    quint32 bitrate() const;
    quint32 sampleRate() const;
    int offset() const;
    bool isValid() const;
    const QMap<Qmmp::MetaData, QString> &metaData() const;

private:
    void parseADTS();
    void parseID3v2(const QByteArray &data);

    qint64 m_duration = 0;
    quint32 m_bitrate = 0;
    int m_offset = 0;
    QIODevice *m_input;
    bool m_isValid = false;
    quint32 m_sampleRate = 0;
    QMap<Qmmp::MetaData, QString> m_metaData;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class ID3v2Tag : public TagLib::ID3v2::Tag
{
public:
    explicit ID3v2Tag(const QByteArray &array);

protected:
    void read();

private:
    QByteArray m_buf;

};

#endif
