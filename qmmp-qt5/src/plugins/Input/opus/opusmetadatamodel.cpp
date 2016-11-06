/***************************************************************************
 *   Copyright (C) 2013-2016 by Ilya Kotov                                 *
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

#include <QtGlobal>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/opusfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/tmap.h>
#include "opusmetadatamodel.h"

OpusMetaDataModel::OpusMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_path = path;
    m_file = new TagLib::Ogg::Opus::File(QStringToFileName(m_path));
    m_tags << new VorbisCommentModel(m_file);
}

OpusMetaDataModel::~OpusMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    if(m_file)
    {
        delete m_file;
        m_file = 0;
    }
}

QHash<QString, QString> OpusMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    TagLib::Ogg::Opus::File f (QStringToFileName(m_path));
    if(m_file && m_file->isValid())
    {
        QString text = QString("%1").arg(m_file->audioProperties()->length()/60);
        text +=":"+QString("%1").arg(m_file->audioProperties()->length()%60,2,10,QChar('0'));
        ap.insert(tr("Length"), text);
        ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg(m_file->audioProperties()->sampleRate()));
        ap.insert(tr("Channels"), QString("%1").arg(m_file->audioProperties()->channels()));
        ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps")).arg(m_file->audioProperties()->bitrate()));
        ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(m_file->length()/1024));
    }
    return ap;
}

QList<TagModel* > OpusMetaDataModel::tags()
{
    return m_tags;
}

QPixmap OpusMetaDataModel::cover()
{
    if(!m_file || !m_file->isValid())
        return QPixmap();

    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(!tag)
        return QPixmap();
    TagLib::StringList list = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
    if(list.isEmpty())
        return QPixmap();
    for(uint i = 0; i < list.size(); ++i)
    {
        TagLib::String value = list[i];
        QByteArray block = QByteArray::fromBase64(TStringToQString(value).toLatin1());
        if(block.size() < 32)
            continue;
        qint64 pos = 0;
        if(readPictureBlockField(block, pos) != 3) //picture type, use front cover only
            continue;
        pos += 4;
        int mimeLength = readPictureBlockField(block, pos); //mime type length
        pos += 4;
        pos += mimeLength; //skip mime type
        int descLength = readPictureBlockField(block, pos); //description length
        pos += 4;
        pos += descLength; //skip description
        pos += 4; //width
        pos += 4; //height
        pos += 4; //color depth
        pos += 4; //the number of colors used
        int length = readPictureBlockField(block, pos); //picture size
        pos += 4;
        QPixmap cover;
        cover.loadFromData(block.mid(pos, length)); //read binary picture data
        return cover;
    }
    return QPixmap();
}

ulong OpusMetaDataModel::readPictureBlockField(QByteArray data, int offset)
{
    return (((uchar)data.data()[offset] & 0xff) << 24) |
           (((uchar)data.data()[offset+1] & 0xff) << 16) |
           (((uchar)data.data()[offset+2] & 0xff) << 16) |
           ((uchar)data.data()[offset+3] & 0xff);

}

VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::Opus::File *file) : TagModel(TagModel::Save)
{
    m_file = file;
    m_tag = file->tag();
}

VorbisCommentModel::~VorbisCommentModel()
{}

const QString VorbisCommentModel::name()
{
    return "Vorbis Comment";
}

const QString VorbisCommentModel::value(Qmmp::MetaData key)
{
    if(!m_tag)
        return QString();
    switch((int) key)
    {
    case Qmmp::TITLE:
        return TStringToQString(m_tag->title());
    case Qmmp::ARTIST:
        return TStringToQString(m_tag->artist());
    case Qmmp::ALBUMARTIST:
        if(m_tag->fieldListMap()["ALBUMARTIST"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["ALBUMARTIST"].front());
    case Qmmp::ALBUM:
        return TStringToQString(m_tag->album());
    case Qmmp::COMMENT:
        return TStringToQString(m_tag->comment());
    case Qmmp::GENRE:
        return TStringToQString(m_tag->genre());
    case Qmmp::COMPOSER:
        if(m_tag->fieldListMap()["COMPOSER"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["COMPOSER"].front());
    case Qmmp::YEAR:
        return QString::number(m_tag->year());
    case Qmmp::TRACK:
        return QString::number(m_tag->track());
    case  Qmmp::DISCNUMBER:
        if(m_tag->fieldListMap()["DISCNUMBER"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["DISCNUMBER"].front());
    }
    return QString();
}

void VorbisCommentModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;

    TagLib::String str = TagLib::String(value.toUtf8().data(), TagLib::String::UTF8);

    switch((int) key)
    {
    case Qmmp::TITLE:
        m_tag->setTitle(str);
        return;
    case Qmmp::ARTIST:
        m_tag->setArtist(str);
        return;
    case Qmmp::ALBUMARTIST:
        m_tag->addField("ALBUMARTIST", str, true);
        return;
    case Qmmp::ALBUM:
        m_tag->setAlbum(str);
        return;
    case Qmmp::COMMENT:
        m_tag->setComment(str);
        return;
    case Qmmp::GENRE:
        m_tag->setGenre(str);
        return;
    case Qmmp::COMPOSER:
        m_tag->addField("COMPOSER", str, true);
        return;
    case Qmmp::TRACK:
        m_tag->setTrack(value.toInt());
        return;
    case Qmmp::YEAR:
        m_tag->setYear(value.toInt());
        return;
    case Qmmp::DISCNUMBER:
        value == "0" ?
        m_tag->removeField("DISCNUMBER"):
        m_tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_file)
        m_file->save();
}
