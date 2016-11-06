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

#include <QTextCodec>
#include <QSettings>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/apetag.h>
#include <taglib/tfile.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegheader.h>
#include <taglib/mpegproperties.h>
#include <taglib/textidentificationframe.h>
#include <taglib/attachedpictureframe.h>

#include "mpegmetadatamodel.h"

MPEGMetaDataModel::MPEGMetaDataModel(bool using_rusxmms, const QString &path, QObject *parent) : MetaDataModel(parent)
{
    m_file = new TagLib::MPEG::File(QStringToFileName(path));
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::ID3v1);
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::ID3v2);
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::APE);
}

MPEGMetaDataModel::~MPEGMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_file;
}

QHash<QString, QString> MPEGMetaDataModel::audioProperties()
{
    QHash<QString, QString> ap;
    QString text;
    QString v;
    switch((int)m_file->audioProperties()->version())
    {
        case TagLib::MPEG::Header::Version1:
        v = "1";
        break;
        case TagLib::MPEG::Header::Version2:
        v = "2";
        break;
        case TagLib::MPEG::Header::Version2_5:
        v = "2.5";
    }
    text = QString("MPEG-%1 layer %2").arg(v).arg(m_file->audioProperties()->layer());
    ap.insert(tr("Format"), text);
    text = QString("%1").arg(m_file->audioProperties()->bitrate());
    ap.insert(tr("Bitrate"), text+" "+tr("kbps"));
    text = QString("%1").arg(m_file->audioProperties()->sampleRate());
    ap.insert(tr("Sample rate"), text+" "+tr("Hz"));
    switch (m_file->audioProperties()->channelMode())
    {
    case TagLib::MPEG::Header::Stereo:
        ap.insert(tr("Mode"), "Stereo");
        break;
    case TagLib::MPEG::Header::JointStereo:
        ap.insert(tr("Mode"), "Joint stereo");
        break;
    case TagLib::MPEG::Header::DualChannel:
        ap.insert(tr("Mode"), "Dual channel");
        break;
    case TagLib::MPEG::Header::SingleChannel:
        ap.insert(tr("Mode"), "Single channel");
        break;
    }
    text = QString("%1 "+tr("KB")).arg(m_file->length()/1024);
    ap.insert(tr("File size"), text);
    if (m_file->audioProperties()->protectionEnabled())
        ap.insert(tr("Protection"), tr("Yes"));
    else
        ap.insert(tr("Protection"), tr("No"));
    if (m_file->audioProperties()->isCopyrighted())
        ap.insert(tr("Copyright"), tr("Yes"));
    else
        ap.insert(tr("Copyright"), tr("No"));
    if (m_file->audioProperties()->isOriginal())
        ap.insert(tr("Original"), tr("Yes"));
    else
        ap.insert(tr("Original"), tr("No"));
    return ap;
}

QList<TagModel* > MPEGMetaDataModel::tags()
{
    return m_tags;
}

QPixmap MPEGMetaDataModel::cover()
{
    if(!m_file->ID3v2Tag())
        return QPixmap();
    TagLib::ID3v2::FrameList frames = m_file->ID3v2Tag()->frameListMap()["APIC"];
    if(frames.isEmpty())
        return QPixmap();

    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        TagLib::ID3v2::AttachedPictureFrame *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
        if(frame && frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
        {
            QPixmap cover;
            cover.loadFromData((const uchar *)frame->picture().data(),
                                     frame->picture().size());
            return cover;
        }
    }
    //fallback image
    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        TagLib::ID3v2::AttachedPictureFrame *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
        if(frame)
        {
            QPixmap cover;
            cover.loadFromData((const uchar *)frame->picture().data(),
                                     frame->picture().size());
            return cover;
        }
    }
    return QPixmap();
}

MpegFileTagModel::MpegFileTagModel(bool using_rusxmms, TagLib::MPEG::File *file, TagLib::MPEG::File::TagTypes tagType)
        : TagModel()
{
    m_tagType = tagType;
    m_file = file;
    m_using_rusxmms = using_rusxmms;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MAD");
    if (m_tagType == TagLib::MPEG::File::ID3v1)
    {
        m_tag = m_file->ID3v1Tag();
        m_codec = QTextCodec::codecForName(settings.value("ID3v1_encoding", "ISO-8859-1").toByteArray ());
        if (!m_codec)
            QTextCodec::codecForName ("ISO-8859-1");
    }
    else if (m_tagType == TagLib::MPEG::File::ID3v2)
    {
        m_tag = m_file->ID3v2Tag();
        m_codec = QTextCodec::codecForName(settings.value("ID3v2_encoding", "UTF-8").toByteArray ());
        if (!m_codec)
            QTextCodec::codecForName ("UTF-8");
    }
    else
    {
        m_tag = m_file->APETag();
        m_codec = QTextCodec::codecForName ("UTF-8");
    }
    if(m_using_rusxmms)
    {
        m_codec = QTextCodec::codecForName ("UTF-8");
    }
    settings.endGroup();
}

MpegFileTagModel::~MpegFileTagModel()
{}

const QString MpegFileTagModel::name()
{
    if (m_tagType == TagLib::MPEG::File::ID3v1)
        return "ID3v1";
    else if (m_tagType == TagLib::MPEG::File::ID3v2)
        return "ID3v2";
    return "APE";
}

QList<Qmmp::MetaData> MpegFileTagModel::keys()
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    if (m_tagType == TagLib::MPEG::File::ID3v2)
        return list;
    else if(m_tagType == TagLib::MPEG::File::APE)
    {
        list.removeAll(Qmmp::DISCNUMBER);
        return list;
    }
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    return list;
}

const QString MpegFileTagModel::value(Qmmp::MetaData key)
{
    QTextCodec *codec = m_codec;

    if (m_tag)
    {
        bool utf = codec->name().contains("UTF");
        if (utf)
            codec = QTextCodec::codecForName ("UTF-8");

        TagLib::String str;
        switch((int) key)
        {
        case Qmmp::TITLE:
            str = m_tag->title();
            break;
        case Qmmp::ARTIST:
            str = m_tag->artist();
            break;
        case Qmmp::ALBUMARTIST:
            if(m_tagType == TagLib::MPEG::File::ID3v2 &&
                    !m_file->ID3v2Tag()->frameListMap()["TPE2"].isEmpty())
            {
                str = m_file->ID3v2Tag()->frameListMap()["TPE2"].front()->toString();
            }
            else if(m_tagType == TagLib::MPEG::File::APE &&
                    !m_file->APETag()->itemListMap()["ALBUM ARTIST"].isEmpty())
            {
                str = m_file->APETag()->itemListMap()["ALBUM ARTIST"].toString();
            }
            break;
        case Qmmp::ALBUM:
            str = m_tag->album();
            break;
        case Qmmp::COMMENT:
            str = m_tag->comment();
            break;
        case Qmmp::GENRE:
            str = m_tag->genre();
            break;
        case Qmmp::COMPOSER:
            if(m_tagType == TagLib::MPEG::File::ID3v2 &&
                    !m_file->ID3v2Tag()->frameListMap()["TCOM"].isEmpty())
            {
                str = m_file->ID3v2Tag()->frameListMap()["TCOM"].front()->toString();
            }
            else if(m_tagType == TagLib::MPEG::File::APE &&
                    !m_file->APETag()->itemListMap()["COMPOSER"].isEmpty())
            {
                str = m_file->APETag()->itemListMap()["COMPOSER"].toString();
            }
            break;
        case Qmmp::YEAR:
            return QString::number(m_tag->year());
        case Qmmp::TRACK:
            return QString::number(m_tag->track());
        case  Qmmp::DISCNUMBER:
            if(m_tagType == TagLib::MPEG::File::ID3v2
               && !m_file->ID3v2Tag()->frameListMap()["TPOS"].isEmpty())
                str = m_file->ID3v2Tag()->frameListMap()["TPOS"].front()->toString();
        }
        return codec->toUnicode(str.toCString(utf)).trimmed();
    }
    return QString();
}

void MpegFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;
    TagLib::String::Type type = TagLib::String::Latin1;

    if (m_tagType == TagLib::MPEG::File::ID3v1)
    {
        if(m_codec->name().contains("UTF") && !m_using_rusxmms) //utf is unsupported
            return;
    }
    else if (m_tagType == TagLib::MPEG::File::ID3v2)
    {
        if (m_codec->name().contains("UTF"))
        {
            type = TagLib::String::UTF8;
            if (m_codec->name().contains("UTF-16"))
                type = TagLib::String::UTF16;
            else if (m_codec->name().contains("UTF-16LE"))
                type = TagLib::String::UTF16LE;
            else if (m_codec->name().contains("UTF-16BE"))
                type = TagLib::String::UTF16BE;

            m_codec = QTextCodec::codecForName ("UTF-8");
            TagLib::ID3v2::FrameFactory *factory = TagLib::ID3v2::FrameFactory::instance();
            factory->setDefaultTextEncoding(type);
            m_file->setID3v2FrameFactory(factory);
            type = TagLib::String::UTF8;
        }
        //save additional tags
        TagLib::ByteVector id3v2_key;
        if(key == Qmmp::ALBUMARTIST)
            id3v2_key = "TPE2"; //album artist
        else if (key == Qmmp::COMPOSER)
            id3v2_key = "TCOM"; //composer
        else if (key == Qmmp::DISCNUMBER)
            id3v2_key = "TPOS";  //disc number

        if (!id3v2_key.isEmpty())
        {
            TagLib::String composer = TagLib::String(m_codec->fromUnicode(value).constData(), type);
            TagLib::ID3v2::Tag *id3v2_tag = dynamic_cast<TagLib::ID3v2::Tag *>(m_tag);
            if(value.isEmpty())
                id3v2_tag->removeFrames(id3v2_key);
            else if(!id3v2_tag->frameListMap()[id3v2_key].isEmpty())
                id3v2_tag->frameListMap()[id3v2_key].front()->setText(composer);
            else
            {
                TagLib::ID3v2::TextIdentificationFrame *frame;
                frame = new TagLib::ID3v2::TextIdentificationFrame(id3v2_key, type);
                frame->setText(composer);
                id3v2_tag->addFrame(frame);
            }
            return;
        }
    }
    else if(m_tagType == TagLib::MPEG::File::APE)
    {
        type = TagLib::String::UTF8;
    }

    TagLib::String str = TagLib::String(m_codec->fromUnicode(value).constData(), type);

    if(m_tagType == TagLib::MPEG::File::APE)
    {
        if(key == Qmmp::COMPOSER)
        {
            m_file->APETag()->addValue("COMPOSER", str, true);
            return;
        }
        else if(key == Qmmp::ALBUMARTIST)
        {
            m_file->APETag()->addValue("ALBUM ARTIST", str, true);
            return;
        }
    }

    switch((int) key)
    {
    case Qmmp::TITLE:
        m_tag->setTitle(str);
        break;
    case Qmmp::ARTIST:
        m_tag->setArtist(str);
        break;
    case Qmmp::ALBUM:
        m_tag->setAlbum(str);
        break;
    case Qmmp::COMMENT:
        m_tag->setComment(str);
        break;
    case Qmmp::GENRE:
        m_tag->setGenre(str);
        break;
    case Qmmp::YEAR:
        m_tag->setYear(value.toInt());
        break;
    case Qmmp::TRACK:
        m_tag->setTrack(value.toInt());
    }
}

bool MpegFileTagModel::exists()
{
    return (m_tag != 0);
}

void MpegFileTagModel::create()
{
    if (m_tag)
        return;
    if (m_tagType == TagLib::MPEG::File::ID3v1)
        m_tag = m_file->ID3v1Tag(true);
    else if (m_tagType == TagLib::MPEG::File::ID3v2)
        m_tag = m_file->ID3v2Tag(true);
    else if (m_tagType == TagLib::MPEG::File::APE)
        m_tag = m_file->APETag(true);
}

void MpegFileTagModel::remove()
{
    m_tag = 0;
}

void MpegFileTagModel::save()
{
    if(m_tag)
        m_file->save(m_tagType, false);
    else
        m_file->strip(m_tagType);
}
