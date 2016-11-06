#include "tagreadandwrite.h"

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <QTextCodec>

TagReadAndWrite::TagReadAndWrite(const QString &file)
{
#if QT_VERSION < 0x050000
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    m_path = file;
}

TagReadAndWrite::~TagReadAndWrite()
{
#if QT_VERSION < 0x050000
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
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
        m_parameters[TAG_TITLE] = tag->title().toCString();
        m_parameters[TAG_ARTIST] = tag->artist().toCString();
        m_parameters[TAG_ALBUM] = tag->album().toCString();
        m_parameters[TAG_YEAR] = QString::number(tag->year());
        m_parameters[TAG_COMMENT] = tag->comment().toCString();
        m_parameters[TAG_TRACK] = QString::number(tag->track());
        m_parameters[TAG_GENRE] = tag->genre().toCString();

        TagLib::PropertyMap properties = tagFile.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i = properties.begin(); i != properties.end(); ++i)
        {
            if(i->first == "ENCODER")
            {
                m_parameters[TAG_MODE] = (*i->second.begin()).toCString();
            }
            if(i->first == "COMPATIBLE_BRANDS")
            {
                m_parameters[TAG_FORMAT] = (*i->second.begin()).toCString();
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
            tags->setTitle(value.toStdString());
            break;
        case TAG_ARTIST:
            tags->setArtist(value.toStdString());
            break;
        case TAG_ALBUM:
            tags->setAlbum(value.toStdString());
            break;
        case TAG_YEAR:
            tags->setYear(value.toInt());
            break;
        case TAG_COMMENT:
            tags->setComment(value.toStdString());
            break;
        case TAG_TRACK:
            tags->setTrack(value.toInt());
            break;
        case TAG_GENRE:
            tags->setGenre(value.toStdString());
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
