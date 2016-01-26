/***************************************************************************
 *   Copyright (C) 2009-2015 by Ilya Kotov                                 *
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

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/vorbisfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/tmap.h>
#include "vorbismetadatamodel.h"

#define QStringToTString_qt4(s) TagLib::String(s.toUtf8().constData(), TagLib::String::UTF8)
#define TStringToQString_qt4(s) QString::fromUtf8(s.toCString(true)).trimmed()

VorbisMetaDataModel::VorbisMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_path = path;
    m_tags << new VorbisCommentModel(path);
}

VorbisMetaDataModel::~VorbisMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
}

QHash<QString, QString> VorbisMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    TagLib::Ogg::Vorbis::File f (m_path.toLocal8Bit().constData());
    if(f.audioProperties())
    {
        QString text = QString("%1").arg(f.audioProperties()->length()/60);
        text +=":"+QString("%1").arg(f.audioProperties()->length()%60,2,10,QChar('0'));
        ap.insert(tr("Length"), text);
        ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg(f.audioProperties()->sampleRate()));
        ap.insert(tr("Channels"), QString("%1").arg(f.audioProperties()->channels()));
        ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps")).arg(f.audioProperties()->bitrate()));
        ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(f.length()/1024));
    }
    return ap;
}

QList<TagModel* > VorbisMetaDataModel::tags()
{
    return m_tags;
}

QPixmap VorbisMetaDataModel::cover()
{
    TagLib::Ogg::Vorbis::File file(m_path.toLocal8Bit().constData());
    TagLib::Ogg::XiphComment *tag = file.tag();
    if(!tag)
        return QPixmap();
    TagLib::StringList list = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
    if(list.isEmpty())
        return QPixmap();
    for(uint i = 0; i < list.size(); ++i)
    {
        TagLib::String value = list[i];
        QByteArray block = QByteArray::fromBase64(TStringToQString_qt4(value).toAscii());
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

ulong VorbisMetaDataModel::readPictureBlockField(QByteArray data, int offset)
{
    return (((uchar)data.data()[offset] & 0xff) << 24) |
           (((uchar)data.data()[offset+1] & 0xff) << 16) |
           (((uchar)data.data()[offset+2] & 0xff) << 16) |
           ((uchar)data.data()[offset+3] & 0xff);

}

VorbisCommentModel::VorbisCommentModel(const QString &path) : TagModel(TagModel::Save)
{
    m_file = new TagLib::Ogg::Vorbis::File (path.toLocal8Bit().constData());
    m_tag = m_file->tag();
}

VorbisCommentModel::~VorbisCommentModel()
{
    delete m_file;
}

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
        return TStringToQString_qt4(m_tag->title());
    case Qmmp::ARTIST:
        return TStringToQString_qt4(m_tag->artist());
    case Qmmp::ALBUMARTIST:
        if(m_tag->fieldListMap()["ALBUMARTIST"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(m_tag->fieldListMap()["ALBUMARTIST"].front());
    case Qmmp::ALBUM:
        return TStringToQString_qt4(m_tag->album());
    case Qmmp::COMMENT:
        return TStringToQString_qt4(m_tag->comment());
    case Qmmp::GENRE:
        return TStringToQString_qt4(m_tag->genre());
    case Qmmp::COMPOSER:
        if(m_tag->fieldListMap()["COMPOSER"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(m_tag->fieldListMap()["COMPOSER"].front());
    case Qmmp::YEAR:
        return QString::number(m_tag->year());
    case Qmmp::TRACK:
        return QString::number(m_tag->track());
    case  Qmmp::DISCNUMBER:
        if(m_tag->fieldListMap()["DISCNUMBER"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(m_tag->fieldListMap()["DISCNUMBER"].front());
    }
    return QString();
}

void VorbisCommentModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;

    TagLib::String str = QStringToTString_qt4(value);

    switch((int) key)
    {
    case Qmmp::TITLE:
        m_tag->setTitle(str);
        return;
    case Qmmp::ARTIST:
        m_tag->setArtist(str);
        return;
    case Qmmp::ALBUM:
        m_tag->setAlbum(str);
        return;
    case Qmmp::ALBUMARTIST:
        m_tag->addField("ALBUMARTIST", str, true);
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
    if(m_tag)
        m_file->save();
    //taglib bug workarround
    QString path = QString::fromLocal8Bit(m_file->name());
    delete m_file;
    m_file = new TagLib::Ogg::Vorbis::File(path.toLocal8Bit().constData());
    m_tag = m_file->tag();
}
