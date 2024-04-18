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

#ifndef TAGEXTRACTOR_H
#define TAGEXTRACTOR_H

#include <QMap>

#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>

#include <qmmp/qmmp.h>

class QIODevice;
class QBuffer;
class QByteArray;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class TagExtractor
{
public:
    explicit TagExtractor(QIODevice *input);
    ~TagExtractor();

    const QMap<Qmmp::MetaData, QString> id3v2tag() const;

private:
    QIODevice *m_input;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class ID3v2Tag : public TagLib::ID3v2::Tag
{
public:
    ID3v2Tag(QByteArray *array, long offset);
    ~ID3v2Tag();

protected:
    void read();

private:
    QBuffer *m_buf;
    long m_offset;

};

#endif
