/***************************************************************************
 *   Copyright (C) 2013-2019 by Ilya Kotov                                 *
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

#include <QtGlobal>
#include <QBuffer>
#include <taglib/flacpicture.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tmap.h>
#include "opusmetadatamodel.h"

OpusMetaDataModel::OpusMetaDataModel(const QString &path, bool readOnly)
#ifdef HAS_PICTURE_LIST
    : MetaDataModel(readOnly, MetaDataModel::IS_COVER_EDITABLE)
#else
    : MetaDataModel(readOnly)
#endif
{
    m_path = path;
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);
    m_file = new TagLib::Ogg::Opus::File(m_stream);
    m_tags << new VorbisCommentModel(m_file);
}

OpusMetaDataModel::~OpusMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();

    delete m_file;
    delete m_stream;
}

QList<MetaDataItem> OpusMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    TagLib::Ogg::Opus::Properties *ap = m_file->audioProperties();
    if(ap)
    {
        ep << MetaDataItem(tr("Version"), ap->opusVersion());
    }
    return ep;
}

QList<TagModel* > OpusMetaDataModel::tags() const
{
    return m_tags;
}

QPixmap OpusMetaDataModel::cover() const
{
    if(!m_file || !m_file->isValid())
        return QPixmap();

    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(!tag || tag->isEmpty())
        return QPixmap();

#ifdef HAS_PICTURE_LIST
    TagLib::List<TagLib::FLAC::Picture *> list = tag->pictureList();
    for(uint i = 0; i < list.size(); ++i)
    {
        if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
        {
            QPixmap cover;
            cover.loadFromData(QByteArray(list[i]->data().data(), list[i]->data().size())); //read binary picture data
            return cover;
        }
    }
#else
    TagLib::StringList list = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];
    if(list.isEmpty())
        return QPixmap();
    for(uint i = 0; i < list.size(); ++i)
    {
        TagLib::FLAC::Picture pict;
        TagLib::String value = list[i];
        QByteArray block = QByteArray::fromBase64(TStringToQString(value).toLatin1());
        pict.parse(TagLib::ByteVector(block.constData(), block.size()));
        QPixmap cover;
        cover.loadFromData(QByteArray(pict.data().data(), pict.data().size())); //read binary picture data
        return cover;
    }
#endif
    return QPixmap();
}

#ifdef HAS_PICTURE_LIST
void OpusMetaDataModel::setCover(const QPixmap &pix)
{
    removeCover();
    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(tag)
    {
        TagLib::FLAC::Picture *picture = new TagLib::FLAC::Picture();
        picture->setType(TagLib::FLAC::Picture::FrontCover);

        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "JPEG");
        picture->setMimeType("image/jpeg");
        picture->setDescription("TTK");
        picture->setData(TagLib::ByteVector(data.constData(), data.size()));
        tag->addPicture(picture);
        m_file->save();
    }
}

void OpusMetaDataModel::removeCover()
{
    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(tag && !tag->isEmpty())
    {
        bool save = false;
        TagLib::List<TagLib::FLAC::Picture *> list = tag->pictureList();
        for(uint i = 0; i < list.size(); ++i)
        {
            if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
            {
                tag->removePicture(list[i], false);
                save = true;
            }
        }
        if(save)
        {
            m_file->save();
        }
    }
}
#endif

VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::Opus::File *file) : TagModel(TagModel::Save)
{
    m_file = file;
    m_tag = file->tag();
}

VorbisCommentModel::~VorbisCommentModel()
{}

QString VorbisCommentModel::name() const
{
    return "Vorbis Comment";
}

QString VorbisCommentModel::value(Qmmp::MetaData key) const
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

    TagLib::String str = QStringToTString(value);

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
