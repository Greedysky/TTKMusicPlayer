/***************************************************************************
 *   Copyright (C) 2008-2012 by Ilya Kotov                                 *
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
#ifndef AACFILE_H
#define AACFILE_H


#include <QMap>
#include <QString>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2header.h>

#include <qmmp/qmmp.h>

class QIODevice;
class TagExtractor;
class QBuffer;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class AACFile
{
public:
    AACFile(QIODevice *i, bool metaData = true, bool adts = true);

    ~AACFile();

    qint64 length();
    quint32 bitrate();
    quint32 samplerate();
    int offset() const;
    bool isValid();
    const QMap<Qmmp::MetaData, QString> metaData();

private:
    void parseADTS();
    void parseID3v2();
    qint64 m_length;
    quint32 m_bitrate;
    int m_offset;
    QIODevice *m_input;
    bool m_isValid;
    quint32 m_samplerate;
    QMap<Qmmp::MetaData, QString> m_metaData;
};

class ID3v2Tag : public TagLib::ID3v2::Tag
{
public:
    ID3v2Tag(QByteArray *array, long offset);

protected:
    void read ();

private:
    QBuffer *m_buf;
    long m_offset;
};

#endif
