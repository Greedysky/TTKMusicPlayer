#include <QBuffer>
#include "sndfilemetadatamodel.h"
#include <taglib/attachedpictureframe.h>
#include <taglib/textidentificationframe.h>

SndFileMetaDataModel::SndFileMetaDataModel(const QString &path, bool readOnly) :
    MetaDataModel(readOnly, MetaDataModel::IsCoverEditable)
{
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);

    if(TagLib::RIFF::WAV::File::isSupported(m_stream))
    {
        m_wavFile = new TagLib::RIFF::WAV::File(m_stream);
        m_tags << new SndFileTagModel(m_wavFile);
    }
    else if(TagLib::RIFF::AIFF::File::isSupported(m_stream))
    {
        m_aiffFile = new TagLib::RIFF::AIFF::File(m_stream);
        m_tags << new SndFileTagModel(m_aiffFile);
    }
}

SndFileMetaDataModel::~SndFileMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_wavFile;
    delete m_aiffFile;
    delete m_stream;
}

QList<TagModel*> SndFileMetaDataModel::tags() const
{
    return m_tags;
}

QImage SndFileMetaDataModel::cover() const
{
    TagLib::ID3v2::Tag *tag = nullptr;

    if(m_wavFile)
        tag = m_wavFile->ID3v2Tag();
    else if(m_aiffFile)
        tag = m_aiffFile->tag();

    if(!tag)
        return QImage();

    TagLib::ID3v2::FrameList frames = tag->frameListMap()["APIC"];
    if(frames.isEmpty())
        return QImage();

    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        TagLib::ID3v2::AttachedPictureFrame *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
        if(frame && frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
        {
            QImage cover;
            cover.loadFromData((const uchar *)frame->picture().data(), frame->picture().size());
            return cover;
        }
    }
    //fallback image
    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        TagLib::ID3v2::AttachedPictureFrame *frame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
        if(frame)
        {
            QImage cover;
            cover.loadFromData((const uchar *)frame->picture().data(), frame->picture().size());
            return cover;
        }
    }
    return QImage();
}

void SndFileMetaDataModel::setCover(const QImage &img)
{
    TagLib::ID3v2::Tag *tag = nullptr;

    if(m_wavFile)
        tag = m_wavFile->ID3v2Tag();
    else if(m_aiffFile)
        tag = m_aiffFile->tag();

    if(!tag)
        return;

    tag->removeFrames("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;
    frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);

    QByteArray data;
    QBuffer buffer(&data);
    if(buffer.open(QIODevice::WriteOnly))
    {
        img.save(&buffer, "JPEG");
    }
    frame->setMimeType("image/jpeg");
    frame->setDescription("TTK");
    frame->setPicture(TagLib::ByteVector(data.constData(), data.size()));
    tag->addFrame(frame);

    if(m_wavFile)
        m_wavFile->save(TagLib::RIFF::WAV::File::ID3v2, TagLib::File::StripNone, TagLib::ID3v2::v4);
    else if(m_aiffFile)
        m_aiffFile->save(TagLib::ID3v2::v4);
}

void SndFileMetaDataModel::removeCover()
{
    if(m_wavFile && m_wavFile->ID3v2Tag())
    {
        m_wavFile->ID3v2Tag()->removeFrames("APIC");
        m_wavFile->save(TagLib::RIFF::WAV::File::ID3v2, TagLib::File::StripNone, TagLib::ID3v2::v4);
    }
    else if(m_aiffFile)
    {
         m_aiffFile->tag()->removeFrames("APIC");
         m_aiffFile->save(TagLib::ID3v2::v4);
    }
}

QString SndFileMetaDataModel::lyrics() const
{
    for(const TagModel *tag : std::as_const(m_tags))
    {
        const SndFileTagModel *mpegTag = static_cast<const SndFileTagModel*>(tag);
        QString lyrics = mpegTag->lyrics();
        if(!lyrics.isEmpty())
            return lyrics;
    }

    return QString();
}


SndFileTagModel::SndFileTagModel(TagLib::RIFF::WAV::File *file)
    : m_wavFile(file)
{
    m_tag = m_wavFile->ID3v2Tag();
}

SndFileTagModel::SndFileTagModel(TagLib::RIFF::AIFF::File *file)
    : TagModel(TagModel::Save),
      m_aiffFile(file)
{
    m_tag = m_aiffFile->tag();
}

QString SndFileTagModel::name() const
{
    return "ID3v2";
}

QString SndFileTagModel::value(Qmmp::MetaData key) const
{
    if(!m_tag)
        return QString();

    switch(key)
    {
    case Qmmp::TITLE:
        return TStringToQString(m_tag->title());
    case Qmmp::ARTIST:
        return TStringToQString(m_tag->artist());
    case Qmmp::ALBUMARTIST:
        if(!m_tag->frameListMap()["TPE2"].isEmpty())
        {
            return TStringToQString(m_tag->frameListMap()["TPE2"].front()->toString());
        }
        return QString();
    case Qmmp::ALBUM:
        return TStringToQString(m_tag->album());
    case Qmmp::COMMENT:
        return TStringToQString(m_tag->comment());
    case Qmmp::GENRE:
        return TStringToQString(m_tag->genre());
    case Qmmp::COMPOSER:
        if(!m_tag->frameListMap()["TCOM"].isEmpty())
        {
            return TStringToQString(m_tag->frameListMap()["TCOM"].front()->toString());
        }
        return QString();
    case Qmmp::YEAR:
        return QString::number(m_tag->year());
    case Qmmp::TRACK:
        return QString::number(m_tag->track());
    case  Qmmp::DISCNUMBER:
        if(!m_tag->frameListMap()["TPOS"].isEmpty())
        {
            return TStringToQString(m_tag->frameListMap()["TPOS"].front()->toString());
        }
        return QString();

    default:
        ;
    }
    return QString();
}

void SndFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;

    TagLib::String str = QStringToTString(value);

    switch(key)
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
    case Qmmp::COMMENT:
        m_tag->setComment(str);
        return;
    case Qmmp::GENRE:
        m_tag->setGenre(str);
        return;
    case Qmmp::YEAR:
        m_tag->setYear(value.toInt());
        return;
    case Qmmp::TRACK:
        m_tag->setTrack(value.toInt());
    default:
        ;
    }

    //save additional tags
    TagLib::ByteVector id3v2_key;
    if(key == Qmmp::ALBUMARTIST)
        id3v2_key = "TPE2"; //album artist
    else if(key == Qmmp::COMPOSER)
        id3v2_key = "TCOM"; //composer
    else if(key == Qmmp::DISCNUMBER)
        id3v2_key = "TPOS";  //disc number

    if(!id3v2_key.isEmpty())
    {
        if(value.isEmpty())
            m_tag->removeFrames(id3v2_key);
        else if(!m_tag->frameListMap()[id3v2_key].isEmpty())
            m_tag->frameListMap()[id3v2_key].front()->setText(str);
        else
        {
            TagLib::ID3v2::TextIdentificationFrame *frame;
            frame = new TagLib::ID3v2::TextIdentificationFrame(id3v2_key, TagLib::String::UTF8);
            frame->setText(str);
            m_tag->addFrame(frame);
        }
        return;
    }
}

bool SndFileTagModel::exists() const
{
    return (m_tag != nullptr);
}

void SndFileTagModel::create()
{
    if(m_tag)
        return;

    if(m_wavFile)
        m_tag = m_wavFile->ID3v2Tag();
    else if(m_aiffFile)
        m_tag = m_aiffFile->tag();
}

void SndFileTagModel::remove()
{
    m_tag = nullptr;
}

void SndFileTagModel::save()
{
    if(m_tag)
    {
        if(m_wavFile)
            m_wavFile->save(TagLib::RIFF::WAV::File::ID3v2, TagLib::File::StripNone, TagLib::ID3v2::v4);
        else if(m_aiffFile)
            m_aiffFile->save(TagLib::ID3v2::v4);
    }
    else
    {
        if(m_wavFile)
            m_wavFile->strip(TagLib::RIFF::WAV::File::ID3v2);
    }
}

QString SndFileTagModel::lyrics() const
{
    if(m_tag)
    {
        const TagLib::ID3v2::FrameListMap &items = m_tag->frameListMap();

        if(!items["USLT"].isEmpty())
            return TStringToQString(items["USLT"].front()->toString());

        if(!items["SYLT"].isEmpty())
            return TStringToQString(items["SYLT"].front()->toString());
    }

    return QString();
}

