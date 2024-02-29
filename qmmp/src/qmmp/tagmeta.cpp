#include "tagmeta.h"

#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

TagMeta::TagMeta()
{

}

TagMeta::TagMeta(const QString &file)
    : m_path(file)
{

}

bool TagMeta::read()
{
    if(m_path.isEmpty())
    {
        return false;
    }

    TagLib::FileRef tagFile(QStringToFileName(m_path));
    if(tagFile.isNull())
    {
        return false;
    }

    QMap<Type, QString> parameters;
    if(tagFile.tag())
    {
        TagLib::Tag *tag = tagFile.tag();
        parameters[TITLE] = TStringToQString(tag->title());
        parameters[ARTIST] = TStringToQString(tag->artist());
        parameters[ALBUM] = TStringToQString(tag->album());
        parameters[YEAR] = QString::number(tag->year());
        parameters[COMMENT] =TStringToQString(tag->comment());
        parameters[TRACK] = QString::number(tag->track());
        parameters[GENRE] = TStringToQString(tag->genre());

        TagLib::PropertyMap properties = tagFile.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i = properties.begin(); i != properties.end(); ++i)
        {
            if(i->first == "ENCODER")
            {
                parameters[MODE] = TStringToQString((*i->second.begin()));
            }
            if(i->first == "COMPATIBLE_BRANDS")
            {
                parameters[FORMAT] = TStringToQString((*i->second.begin()));
            }
        }
    }

    if(tagFile.audioProperties())
    {
        TagLib::AudioProperties *properties = tagFile.audioProperties();
        if(properties)
        {
            parameters[BITRATE] = QString("%1 kbps").arg(properties->bitrate());
            parameters[SAMPLERATE] = QString("%1 Hz").arg(properties->sampleRate());
            parameters[CHANNEL] = QString::number(properties->channels());
            parameters[LENGTH] = QString::number(properties->lengthInMilliseconds());
        }
    }

    return true;
}

bool TagMeta::write(Type tag, const QString &value, int id3v2Version)
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
        default:
            break;
    }

#if TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION <= 11
    return file.save(TagLib::MPEG::File::AllTags, true, id3v2Version);
#else
    return file.save(TagLib::MPEG::File::AllTags, TagLib::File::StripOthers, static_cast<TagLib::ID3v2::Version>(id3v2Version));
#endif
}
