#include "tagreadandwrite.h"

#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

#define DECODE_STRING(value) TagLib::String(value.toUtf8().data(), TagLib::String::UTF8)
#define ENCODE_STRING(value) QString::fromUtf8(value.toCString(true))

TagReadAndWrite::TagReadAndWrite()
{

}

TagReadAndWrite::TagReadAndWrite(const QString &file)
{
    m_path = file;
}

TagReadAndWrite::~TagReadAndWrite()
{

}

bool TagReadAndWrite::readFile()
{
    return readFile(m_path);
}

bool TagReadAndWrite::readFile(const QString &path)
{
    if(path.isEmpty())
    {
        return false;
    }

    m_path = path;

    TagLib::FileRef tagFile(path.toLocal8Bit().constData());
    if(tagFile.isNull())
    {
        return false;
    }

    if(tagFile.tag())
    {
        TagLib::Tag *tag = tagFile.tag();
        m_parameters[TAG_TITLE] = ENCODE_STRING(tag->title());
        m_parameters[TAG_ARTIST] = ENCODE_STRING(tag->artist());
        m_parameters[TAG_ALBUM] = ENCODE_STRING(tag->album());
        m_parameters[TAG_YEAR] = QString::number(tag->year());
        m_parameters[TAG_COMMENT] =ENCODE_STRING(tag->comment());
        m_parameters[TAG_TRACK] = QString::number(tag->track());
        m_parameters[TAG_GENRE] = ENCODE_STRING(tag->genre());

        TagLib::PropertyMap properties = tagFile.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i = properties.begin(); i != properties.end(); ++i)
        {
            if(i->first == "ENCODER")
            {
                m_parameters[TAG_MODE] = ENCODE_STRING((*i->second.begin()));
            }
            if(i->first == "COMPATIBLE_BRANDS")
            {
                m_parameters[TAG_FORMAT] = ENCODE_STRING((*i->second.begin()));
            }
        }
    }

    if(tagFile.audioProperties())
    {
        TagLib::AudioProperties *properties = tagFile.audioProperties();
        if(properties)
        {
            m_parameters[TAG_BITRATE] = QString("%1 kbps").arg(properties->bitrate());
            m_parameters[TAG_SAMPLERATE] = QString("%1 Hz").arg(properties->sampleRate());
            m_parameters[TAG_CHANNEL] = QString::number(properties->channels());
            m_parameters[TAG_LENGTH] = QString::number(properties->lengthInMilliseconds());
        }
    }

    return true;
}

bool TagReadAndWrite::writeMusicTag(MusicTag tag, const QString &value, int id3v2Version)
{
    if(m_path.isEmpty())
    {
        return false;
    }

    TagLib::MPEG::File file(m_path.toLocal8Bit().constData());
    if(!file.isValid())
    {
        return false;
    }

    TagLib::ID3v2::Tag *tags = file.ID3v2Tag(true);
    if(!tags)
    {
        return false;
    }

    switch(tag)
    {
        case TAG_TITLE:
            tags->setTitle(DECODE_STRING(value));
            break;
        case TAG_ARTIST:
            tags->setArtist(DECODE_STRING(value));
            break;
        case TAG_ALBUM:
            tags->setAlbum(DECODE_STRING(value));
            break;
        case TAG_YEAR:
            tags->setYear(value.toInt());
            break;
        case TAG_COMMENT:
            tags->setComment(DECODE_STRING(value));
            break;
        case TAG_TRACK:
            tags->setTrack(value.toInt());
            break;
        case TAG_GENRE:
            tags->setGenre(DECODE_STRING(value));
            break;
        default: break;
    }

    if(file.save(TagLib::MPEG::File::AllTags, true, id3v2Version))
    {
        m_parameters[tag] = value;
        return true;
    }
    return false;
}
