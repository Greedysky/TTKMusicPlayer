#include "tagwrapper.h"

#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

TagWrapper::TagWrapper()
{

}

TagWrapper::TagWrapper(const QString &file)
    : m_path(file)
{

}

bool TagWrapper::readFile()
{
    return readFile(m_path);
}

bool TagWrapper::readFile(const QString &path)
{
    if(path.isEmpty())
    {
        return false;
    }

    m_path = path;

    TagLib::FileRef tagFile(QStringToFileName(path));
    if(tagFile.isNull())
    {
        return false;
    }

    if(tagFile.tag())
    {
        TagLib::Tag *tag = tagFile.tag();
        m_parameters[TITLE] = TStringToQString(tag->title());
        m_parameters[ARTIST] = TStringToQString(tag->artist());
        m_parameters[ALBUM] = TStringToQString(tag->album());
        m_parameters[YEAR] = QString::number(tag->year());
        m_parameters[COMMENT] =TStringToQString(tag->comment());
        m_parameters[TRACK] = QString::number(tag->track());
        m_parameters[GENRE] = TStringToQString(tag->genre());

        TagLib::PropertyMap properties = tagFile.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i = properties.begin(); i != properties.end(); ++i)
        {
            if(i->first == "ENCODER")
            {
                m_parameters[MODE] = TStringToQString((*i->second.begin()));
            }
            if(i->first == "COMPATIBLE_BRANDS")
            {
                m_parameters[FORMAT] = TStringToQString((*i->second.begin()));
            }
        }
    }

    if(tagFile.audioProperties())
    {
        TagLib::AudioProperties *properties = tagFile.audioProperties();
        if(properties)
        {
            m_parameters[BITRATE] = QString("%1 kbps").arg(properties->bitrate());
            m_parameters[SAMPLERATE] = QString("%1 Hz").arg(properties->sampleRate());
            m_parameters[CHANNEL] = QString::number(properties->channels());
            m_parameters[LENGTH] = QString::number(properties->lengthInMilliseconds());
        }
    }

    return true;
}

bool TagWrapper::writeMusicTag(Type tag, const QString &value, int id3v2Version)
{
    if(m_path.isEmpty())
    {
        return false;
    }

    TagLib::MPEG::File file(QStringToFileName(m_path));
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
        case TITLE:
            tags->setTitle(QStringToTString(value));
            break;
        case ARTIST:
            tags->setArtist(QStringToTString(value));
            break;
        case ALBUM:
            tags->setAlbum(QStringToTString(value));
            break;
        case YEAR:
            tags->setYear(value.toInt());
            break;
        case COMMENT:
            tags->setComment(QStringToTString(value));
            break;
        case TRACK:
            tags->setTrack(value.toInt());
            break;
        case GENRE:
            tags->setGenre(QStringToTString(value));
            break;
        default: break;
    }

#if ((TAGLIB_MAJOR_VERSION == 1) && (TAGLIB_MINOR_VERSION <= 11))
    if(file.save(TagLib::MPEG::File::AllTags, true, id3v2Version))
#else
    if(file.save(TagLib::MPEG::File::AllTags, TagLib::File::StripOthers, static_cast<TagLib::ID3v2::Version>(id3v2Version)))
#endif
    {
        m_parameters[tag] = value;
        return true;
    }
    return false;
}
