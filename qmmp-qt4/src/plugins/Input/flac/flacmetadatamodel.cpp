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

#include <QPixmap>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tmap.h>
#include <FLAC/all.h>
#include <qmmp/metadatamanager.h>
#include "flacmetadatamodel.h"

#define QStringToTString_qt4(s) TagLib::String(s.toUtf8().constData(), TagLib::String::UTF8)
#define TStringToQString_qt4(s) QString::fromUtf8(s.toCString(true)).trimmed()

FLACMetaDataModel::FLACMetaDataModel(const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_file = 0;

    if(path.startsWith("flac://"))
    {
        QString p = path;
        p.remove("flac://");
        p.remove(QRegExp("#\\d+$"));
        m_path = p;
    }
    else
        m_path = path;

    TagLib::Ogg::XiphComment *tag = 0;

    if(m_path.endsWith(".flac", Qt::CaseInsensitive))
    {
        TagLib::FLAC::File *f = new TagLib::FLAC::File(QStringToFileName(m_path));
        tag = f->xiphComment();
        m_file = f;
    }
    else if(m_path.endsWith(".oga", Qt::CaseInsensitive))
    {
        TagLib::Ogg::FLAC::File *f = new TagLib::Ogg::FLAC::File(QStringToFileName(m_path));
        tag = f->tag();
        m_file = f;
    }

    if(m_file && m_file->isValid() && !path.startsWith("flac://"))
        m_tags << new VorbisCommentModel(tag, m_file);
}

FLACMetaDataModel::~FLACMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    if(m_file)
    {
        delete m_file;
        m_file = 0;
    }
}

QHash<QString, QString> FLACMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    TagLib::AudioProperties *taglib_ap = 0;
    qint64 size = 0;

    if(m_file && m_file->isValid())
    {
        taglib_ap =  m_file->audioProperties();
        size = m_file->length();
    }

    if(taglib_ap)
    {
        QString text = QString("%1").arg(taglib_ap->length()/60);
        text +=":"+QString("%1").arg(taglib_ap->length()%60,2,10,QChar('0'));
        ap.insert(tr("Length"), text);
        ap.insert(tr("Sample rate"), QString("%1 " + tr("Hz")).arg(taglib_ap->sampleRate()));
        ap.insert(tr("Channels"), QString("%1").arg(taglib_ap->channels()));
        ap.insert(tr("Bitrate"), QString("%1 " + tr("kbps")).arg(taglib_ap->bitrate()));
        ap.insert(tr("File size"), QString("%1 "+tr("KB")).arg(size/1000));
    }
    return ap;
}

QList<TagModel* > FLACMetaDataModel::tags()
{
    return m_tags;
}

QPixmap FLACMetaDataModel::cover()
{
    //embedded cover
    FLAC__StreamMetadata *metadata;
    FLAC__metadata_get_picture (qPrintable(m_path),
                                &metadata,
                                FLAC__STREAM_METADATA_PICTURE_TYPE_FRONT_COVER,
                                0,0, -1,-1,-1,-1);
    if(metadata)
    {
        FLAC__StreamMetadata_Picture *pict = &metadata->data.picture;
        QPixmap cover;
        cover.loadFromData(QByteArray((char *)pict->data, (int) pict->data_length));
        FLAC__metadata_object_delete(metadata);
        return cover;
    }
    QString cPath = coverPath();
    return cPath.isEmpty() ? QPixmap() : QPixmap(cPath);
}

QString FLACMetaDataModel::coverPath()
{
    return MetaDataManager::instance()->getCoverPath(m_path);
}

VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::XiphComment *tag, TagLib::File *file) : TagModel(TagModel::Save)
{
    m_file = file;
    m_tag = tag;
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
