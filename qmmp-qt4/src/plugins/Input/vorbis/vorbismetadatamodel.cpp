/***************************************************************************
 *   Copyright (C) 2009-2016 by Ilya Kotov                                 *
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
    m_file = new TagLib::Ogg::Vorbis::File(QStringToFileName(path));
    m_tag = m_file->tag();
    m_tags << new VorbisCommentModel(this);
}

VorbisMetaDataModel::~VorbisMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();

    if(m_file)
    {
        delete m_file;
        m_file = 0;
    }
}

QHash<QString, QString> VorbisMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    if(m_file->audioProperties())
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

QList<TagModel* > VorbisMetaDataModel::tags()
{
    return m_tags;
}

QPixmap VorbisMetaDataModel::cover()
{
    if(!m_tag || m_tag->isEmpty())
        return QPixmap();
    TagLib::StringList list = m_tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
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

VorbisCommentModel::VorbisCommentModel(VorbisMetaDataModel *model) : TagModel(TagModel::Save)
{
    m_model = model;
}

VorbisCommentModel::~VorbisCommentModel()
{}

const QString VorbisCommentModel::name()
{
    return "Vorbis Comment";
}

const QString VorbisCommentModel::value(Qmmp::MetaData key)
{
    if(!m_model->m_tag || m_model->m_tag->isEmpty())
        return QString();

    TagLib::Ogg::XiphComment *tag  = m_model->m_tag;
    switch((int) key)
    {
    case Qmmp::TITLE:
        return TStringToQString_qt4(tag->title());
    case Qmmp::ARTIST:
        return TStringToQString_qt4(tag->artist());
    case Qmmp::ALBUMARTIST:
        if(tag->fieldListMap()["ALBUMARTIST"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(tag->fieldListMap()["ALBUMARTIST"].front());
    case Qmmp::ALBUM:
        return TStringToQString_qt4(tag->album());
    case Qmmp::COMMENT:
        return TStringToQString_qt4(tag->comment());
    case Qmmp::GENRE:
        return TStringToQString_qt4(tag->genre());
    case Qmmp::COMPOSER:
        if(tag->fieldListMap()["COMPOSER"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(tag->fieldListMap()["COMPOSER"].front());
    case Qmmp::YEAR:
        return QString::number(tag->year());
    case Qmmp::TRACK:
        return QString::number(tag->track());
    case  Qmmp::DISCNUMBER:
        if(tag->fieldListMap()["DISCNUMBER"].isEmpty())
            return QString();
        else
            return TStringToQString_qt4(tag->fieldListMap()["DISCNUMBER"].front());
    }
    return QString();
}

void VorbisCommentModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_model->m_tag || m_model->m_tag->isEmpty())
        return;

    TagLib::Ogg::XiphComment *tag = m_model->m_tag;

    TagLib::String str = QStringToTString_qt4(value);

    switch((int) key)
    {
    case Qmmp::TITLE:
        tag->setTitle(str);
        return;
    case Qmmp::ARTIST:
        tag->setArtist(str);
        return;
    case Qmmp::ALBUM:
        tag->setAlbum(str);
        return;
    case Qmmp::ALBUMARTIST:
        tag->addField("ALBUMARTIST", str, true);
        return;
    case Qmmp::COMMENT:
        tag->setComment(str);
        return;
    case Qmmp::GENRE:
        tag->setGenre(str);
        return;
    case Qmmp::COMPOSER:
        tag->addField("COMPOSER", str, true);
        return;
    case Qmmp::TRACK:
        tag->setTrack(value.toInt());
        return;
    case Qmmp::YEAR:
        tag->setYear(value.toInt());
        return;
    case Qmmp::DISCNUMBER:
        value == "0" ?
        tag->removeField("DISCNUMBER"):
        tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_model->m_tag)
        m_model->m_file->save();

#if ((TAGLIB_MAJOR_VERSION == 1) && (TAGLIB_MINOR_VERSION <= 10))
    //taglib bug workarround
    delete m_model->m_file;
    m_model->m_file = new TagLib::Ogg::Vorbis::File(QStringToFileName(m_model->m_path));
    m_model->m_tag = m_model->m_file->tag();
#endif
}
