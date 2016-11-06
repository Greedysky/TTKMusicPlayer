#include "tagreadandwrite.h"

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

TagReadAndWrite::TagReadAndWrite(const QString &file)
{
    m_path = file;
}

TagReadAndWrite::~TagReadAndWrite()
{

}

bool TagReadAndWrite::readFile()
{
    TagLib::FileRef tagFile(m_path.toLocal8Bit().constData());
    if(tagFile.isNull())
    {
        return false;
    }

    if(tagFile.tag())
    {
        TagLib::Tag *tag = tagFile.tag();
        m_parameters[TAG_TITLE] = QString::fromLocal8Bit(tag->title().toCString());
        m_parameters[TAG_ARTIST] = QString::fromLocal8Bit(tag->artist().toCString());
        m_parameters[TAG_ALBUM] = QString::fromLocal8Bit(tag->album().toCString());
        m_parameters[TAG_YEAR] = QString::number(tag->year());
        m_parameters[TAG_COMMENT] = QString::fromLocal8Bit(tag->comment().toCString());
        m_parameters[TAG_TRACK] = QString::number(tag->track());
        m_parameters[TAG_GENRE] = QString::fromLocal8Bit(tag->genre().toCString());

        TagLib::PropertyMap properties = tagFile.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i = properties.begin(); i != properties.end(); ++i)
        {
            if(i->first == "ENCODER")
            {
                m_parameters[TAG_MODE] = QString::fromLocal8Bit((*i->second.begin()).toCString());
            }
            if(i->first == "COMPATIBLE_BRANDS")
            {
                m_parameters[TAG_FORMAT] = QString::fromLocal8Bit((*i->second.begin()).toCString());
            }
        }
    }

    if(tagFile.audioProperties())
    {
        TagLib::AudioProperties *properties = tagFile.audioProperties();
        m_parameters[TAG_BITRATE] = QString("%1 kbps").arg(properties->bitrate());
        m_parameters[TAG_SAMPLERATE] = QString("%1 Hz").arg(properties->sampleRate());
        m_parameters[TAG_CHANNEL] = QString::number(properties->channels());
        m_parameters[TAG_LENGTH] = QString::number(properties->length()*1000);
    }

    return true;
}

bool TagReadAndWrite::writeMusicTag(MusicTag tag, const QString &value)
{
    TagLib::FileRef tagFile(m_path.toLocal8Bit().constData());
    if(tagFile.isNull())
    {
        return false;
    }

    if(!tagFile.tag())
    {
        return false;
    }

    TagLib::Tag *tags = tagFile.tag();
    switch(tag)
    {
        case TAG_TITLE:
            tags->setTitle(value.toLocal8Bit().constData());
            break;
        case TAG_ARTIST:
            tags->setArtist(value.toLocal8Bit().constData());
            break;
        case TAG_ALBUM:
            tags->setAlbum(value.toLocal8Bit().constData());
            break;
        case TAG_YEAR:
            tags->setYear(value.toInt());
            break;
        case TAG_COMMENT:
            tags->setComment(value.toLocal8Bit().constData());
            break;
        case TAG_TRACK:
            tags->setTrack(value.toInt());
            break;
        case TAG_GENRE:
            tags->setGenre(value.toLocal8Bit().constData());
            break;
        default: break;
    }

    if(tagFile.save())
    {
        m_parameters[tag] = value;
        return true;
    }
    return false;
}
