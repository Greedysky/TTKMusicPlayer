#include "mpegmetadatamodel.h"
#include "tagextractor.h"
#include <qmmp/qmmptextcodec.h>

#include <QBuffer>
#include <QSettings>
#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/popularimeterframe.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/textidentificationframe.h>

MPEGMetaDataModel::MPEGMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable)
{
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);
#if TAGLIB_MAJOR_VERSION >= 2
    m_file = new TagLib::MPEG::File(m_stream);
#else
    m_file = new TagLib::MPEG::File(m_stream, TagLib::ID3v2::FrameFactory::instance());
#endif
    m_tags << new MpegFileTagModel(m_file, TagLib::MPEG::File::ID3v1);
    m_tags << new MpegFileTagModel(m_file, TagLib::MPEG::File::ID3v2);
    m_tags << new MpegFileTagModel(m_file, TagLib::MPEG::File::APE);
}

MPEGMetaDataModel::~MPEGMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_file;
    delete m_stream;
}

QList<MetaDataItem> MPEGMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    TagLib::MPEG::Properties *ap = m_file->audioProperties();
    if(!ap)
    {
        return ep;
    }

    switch(ap->channelMode())
    {
    case TagLib::MPEG::Header::Stereo:
        ep << MetaDataItem(tr("Mode"), "Stereo");
        break;
    case TagLib::MPEG::Header::JointStereo:
        ep << MetaDataItem(tr("Mode"), "Joint stereo");
        break;
    case TagLib::MPEG::Header::DualChannel:
        ep << MetaDataItem(tr("Mode"), "Dual channel");
        break;
    case TagLib::MPEG::Header::SingleChannel:
        ep << MetaDataItem(tr("Mode"), "Single channel");
        break;
    }

    ep << MetaDataItem(tr("Protection"), ap->protectionEnabled());
    ep << MetaDataItem(tr("Copyright"), ap->isCopyrighted());
    ep << MetaDataItem(tr("Original"), ap->isOriginal());

    const TagLib::ID3v2::Tag *tag = m_file->ID3v2Tag();
    if(tag && !tag->frameListMap()["POPM"].isEmpty())
    {
        const TagLib::ID3v2::PopularimeterFrame *popm = dynamic_cast<TagLib::ID3v2::PopularimeterFrame*>(tag->frameListMap()["POPM"].front());
        if(popm)
            ep << MetaDataItem(tr("Rating"), popm->rating());
    }
    return ep;
}

QList<TagModel*> MPEGMetaDataModel::tags() const
{
    return m_tags;
}

QImage MPEGMetaDataModel::cover() const
{
    if(!m_file->ID3v2Tag())
        return QImage();

    TagLib::ID3v2::FrameList frames = m_file->ID3v2Tag()->frameListMap()["APIC"];
    if(frames.isEmpty())
        return QImage();

    for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
    {
        TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
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
        TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);
        if(frame)
        {
            QImage cover;
            cover.loadFromData((const uchar *)frame->picture().data(), frame->picture().size());
            return cover;
        }
    }
    return QImage();
}

void MPEGMetaDataModel::setCover(const QImage &img)
{
    TagLib::ID3v2::Tag *tag = m_file->ID3v2Tag(true);
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
    frame->setPicture(TagLib::ByteVector(data.constData(), data.length()));
    tag->addFrame(frame);
#if TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION <= 11
    m_file->save(TagLib::MPEG::File::ID3v2, true, 3);
#else
    m_file->save(TagLib::MPEG::File::ID3v2, TagLib::File::StripOthers, TagLib::ID3v2::Version::v3);
#endif
}

void MPEGMetaDataModel::removeCover()
{
    if(m_file->ID3v2Tag())
    {
        m_file->ID3v2Tag()->removeFrames("APIC");
#if TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION <= 11
        m_file->save(TagLib::MPEG::File::ID3v2, true, 3);
#else
        m_file->save(TagLib::MPEG::File::ID3v2, TagLib::File::StripOthers, TagLib::ID3v2::Version::v3);
#endif
    }
}

QString MPEGMetaDataModel::lyrics() const
{
    for(const TagModel *tag : qAsConst(m_tags))
    {
        const MpegFileTagModel *mpegTag = static_cast<const MpegFileTagModel*>(tag);
        const QString &lyrics = mpegTag->lyrics();
        if(!lyrics.isEmpty())
            return lyrics;
    }
    return QString();
}


MpegFileTagModel::MpegFileTagModel(TagLib::MPEG::File *file, TagLib::MPEG::File::TagTypes type)
    : TagModel(),
      m_file(file),
      m_type(type)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");

    QByteArray codecName;
    if(m_type == TagLib::MPEG::File::ID3v1)
    {
        m_tag = m_file->ID3v1Tag();
        if((codecName = settings.value("ID3v1_encoding", "GB18030").toByteArray()).isEmpty())
            codecName = "GB18030";
    }
    else if(m_type == TagLib::MPEG::File::ID3v2)
    {
        m_tag = m_file->ID3v2Tag();
        if((codecName = settings.value("ID3v2_encoding", "UTF-8").toByteArray()).isEmpty())
            codecName = "UTF-8";
    }
    else
    {
        m_tag = m_file->APETag();
        codecName = "UTF-8";
    }

    if(codecName.contains("UTF") || codecName.isEmpty())
        codecName = "UTF-8";

    if(m_tag && (m_type == TagLib::MPEG::File::ID3v1 || m_type == TagLib::MPEG::File::ID3v2) && settings.value("detect_encoding", false).toBool())
    {
        QByteArray detectedCharset = TagExtractor::detectCharset(m_tag);
        if(!detectedCharset.isEmpty())
            codecName = detectedCharset;
    }

    m_codec = new QmmpTextCodec(codecName);
    settings.endGroup();
}

QString MpegFileTagModel::name() const
{
    if(m_type == TagLib::MPEG::File::ID3v1)
        return "ID3v1";
    else if(m_type == TagLib::MPEG::File::ID3v2)
        return "ID3v2";
    return "APE";
}

QList<Qmmp::MetaData> MpegFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    if(m_type == TagLib::MPEG::File::ID3v2)
        return list;

    else if(m_type == TagLib::MPEG::File::APE)
    {
        list.removeAll(Qmmp::DISCNUMBER);
        return list;
    }

    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    return list;
}

QString MpegFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_tag)
    {
        const bool utf = m_codec->name().contains("UTF");
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
            if(m_type == TagLib::MPEG::File::ID3v2 && !m_file->ID3v2Tag()->frameListMap()["TPE2"].isEmpty())
            {
                str = m_file->ID3v2Tag()->frameListMap()["TPE2"].front()->toString();
            }
            else if(m_type == TagLib::MPEG::File::APE && !m_file->APETag()->itemListMap()["ALBUM ARTIST"].isEmpty())
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
            if(m_type == TagLib::MPEG::File::ID3v2 && !m_file->ID3v2Tag()->frameListMap()["TCOM"].isEmpty())
            {
                str = m_file->ID3v2Tag()->frameListMap()["TCOM"].front()->toString();
            }
            else if(m_type == TagLib::MPEG::File::APE && !m_file->APETag()->itemListMap()["COMPOSER"].isEmpty())
            {
                str = m_file->APETag()->itemListMap()["COMPOSER"].toString();
            }
            break;
        case Qmmp::YEAR:
            return QString::number(m_tag->year());
        case Qmmp::TRACK:
            return QString::number(m_tag->track());
        case Qmmp::DISCNUMBER:
            if(m_type == TagLib::MPEG::File::ID3v2 && !m_file->ID3v2Tag()->frameListMap()["TPOS"].isEmpty())
                str = m_file->ID3v2Tag()->frameListMap()["TPOS"].front()->toString();
            break;
        }
        return CSTR_TO_QSTR(m_codec, str, utf);
    }
    return QString();
}

void MpegFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;

    TagLib::String::Type type = TagLib::String::Latin1;
    if(m_type == TagLib::MPEG::File::ID3v1)
    {
        if(m_codec->name().contains("UTF")) //utf is unsupported
            return;
    }
    else if(m_type == TagLib::MPEG::File::ID3v2)
    {
        if(m_codec->name().contains("UTF"))
        {
            TagLib::ID3v2::FrameFactory *factory = TagLib::ID3v2::FrameFactory::instance();
            type = TagLib::String::UTF8;
            factory->setDefaultTextEncoding(type);
        }
        else
        {
            TagLib::ID3v2::FrameFactory *factory = TagLib::ID3v2::FrameFactory::instance();
            factory->setDefaultTextEncoding(TagLib::String::Latin1);
        }

        //save additional tags
        TagLib::ByteVector id3v2_key;
        if(key == Qmmp::ALBUMARTIST)
            id3v2_key = "TPE2"; //album artist
        else if(key == Qmmp::COMPOSER)
            id3v2_key = "TCOM"; //composer
        else if(key == Qmmp::DISCNUMBER)
            id3v2_key = "TPOS"; //disc number

        if(!id3v2_key.isEmpty())
        {
            TagLib::String composer = TagLib::String(m_codec->fromUnicode(value).constData(), type);
            TagLib::ID3v2::Tag *id3v2_tag = static_cast<TagLib::ID3v2::Tag*>(m_tag);
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
    else if(m_type == TagLib::MPEG::File::APE)
    {
        type = TagLib::String::UTF8;
    }

    TagLib::String str = TagLib::String(m_codec->fromUnicode(value).constData(), type);

    if(m_type == TagLib::MPEG::File::APE)
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

bool MpegFileTagModel::exists() const
{
    return (m_tag != nullptr);
}

void MpegFileTagModel::create()
{
    if(m_tag)
        return;
    if(m_type == TagLib::MPEG::File::ID3v1)
        m_tag = m_file->ID3v1Tag(true);
    else if(m_type == TagLib::MPEG::File::ID3v2)
        m_tag = m_file->ID3v2Tag(true);
    else if(m_type == TagLib::MPEG::File::APE)
        m_tag = m_file->APETag(true);
}

void MpegFileTagModel::remove()
{
    m_tag = nullptr;
}

void MpegFileTagModel::save()
{
    if(m_tag)
#if TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION <= 11
        m_file->save(m_type, false, 3);
#else
        m_file->save(m_type, TagLib::File::StripNone, TagLib::ID3v2::Version::v3, TagLib::File::DoNotDuplicate);
#endif
    else
        m_file->strip(m_type);
}

QString MpegFileTagModel::lyrics() const
{
    if(m_tag && m_type == TagLib::MPEG::File::ID3v2)
    {
        TagLib::ID3v2::Tag *id3v2_tag = static_cast<TagLib::ID3v2::Tag*>(m_tag);
        const TagLib::ID3v2::FrameListMap &items = id3v2_tag->frameListMap();

        const bool utf = m_codec->name().contains("UTF");
        if(!items["USLT"].isEmpty())
            return m_codec->toUnicode(items["USLT"].front()->toString().toCString(utf));

        else if(!items["SYLT"].isEmpty())
            return m_codec->toUnicode(items["SYLT"].front()->toString().toCString(utf));
    }
    return QString();
}
