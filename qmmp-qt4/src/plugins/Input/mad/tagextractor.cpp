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

#include <QIODevice>
#include <QSettings>
#include <QByteArray>
#include <QBuffer>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include <stdlib.h>

#include "tagextractor.h"

TagExtractor::TagExtractor(QIODevice *d)
{
    m_d = d;
}


TagExtractor::~TagExtractor()
{
}

const QMap<Qmmp::MetaData, QString> TagExtractor::id3v2tag()
{
    QByteArray array = m_d->peek(2048);
    int offset = array.indexOf("ID3");
    if (offset < 0)
        return m_tag;
    ID3v2Tag taglib_tag(&array, offset);
    if (taglib_tag.isEmpty())
        return m_tag;

    TagLib::String album = taglib_tag.album();
    TagLib::String artist = taglib_tag.artist();
    TagLib::String comment = taglib_tag.comment();
    TagLib::String genre = taglib_tag.genre();
    TagLib::String title = taglib_tag.title();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MAD");
    QByteArray name = settings.value("ID3v2_encoding","UTF-8").toByteArray ();
    bool utf = false;
    QTextCodec *codec = 0;
    if (name.contains("UTF"))
    {
        codec = QTextCodec::codecForName ("UTF-8");
        utf = true;
    }
    else
        codec = QTextCodec::codecForName(name);
    settings.endGroup();

    if (!codec)
        codec = QTextCodec::codecForName ("UTF-8");

    m_tag.insert(Qmmp::ALBUM,
                 codec->toUnicode(album.toCString(utf)).trimmed());
    m_tag.insert(Qmmp::ARTIST,
                 codec->toUnicode(artist.toCString(utf)).trimmed());
    m_tag.insert(Qmmp::COMMENT,
                 codec->toUnicode(comment.toCString(utf)).trimmed());
    m_tag.insert(Qmmp::GENRE,
                 codec->toUnicode(genre.toCString(utf)).trimmed());
    m_tag.insert(Qmmp::TITLE,
                 codec->toUnicode(title.toCString(utf)).trimmed());
    m_tag.insert(Qmmp::YEAR,
                 QString::number(taglib_tag.year()));
    m_tag.insert(Qmmp::TRACK,
                 QString::number(taglib_tag.track()));

    if(!taglib_tag.frameListMap()["TCOM"].isEmpty())
    {
        TagLib::String composer = taglib_tag.frameListMap()["TCOM"].front()->toString();
        m_tag.insert(Qmmp::COMPOSER, codec->toUnicode(composer.toCString(utf)).trimmed());
    }
    if(!taglib_tag.frameListMap()["TPOS"].isEmpty())
    {
        TagLib::String disc = taglib_tag.frameListMap()["TPOS"].front()->toString();
        m_tag.insert(Qmmp::DISCNUMBER, QString(disc.toCString()).trimmed());
    }
    return m_tag;
}

ID3v2Tag::ID3v2Tag(QByteArray *array, long offset) : TagLib::ID3v2::Tag()
{
    m_buf = new QBuffer(array);
    m_buf->open(QIODevice::ReadOnly);
    m_offset = offset;
    read();
}

void ID3v2Tag::read ()
{
    m_buf->seek(m_offset);
    uint to_read = TagLib::ID3v2::Header::size();
    if (to_read > 2048 - uint(m_offset))
        return;
    header()->setData(TagLib::ByteVector(m_buf->read(to_read).data(), to_read));
    to_read = header()->tagSize();
    if (!to_read ||  2048 < m_offset + TagLib::ID3v2::Header::size())
        return;
    QByteArray array = m_buf->read(to_read);
    TagLib::ByteVector v(array.data(), array.size());
    parse(v);
}
