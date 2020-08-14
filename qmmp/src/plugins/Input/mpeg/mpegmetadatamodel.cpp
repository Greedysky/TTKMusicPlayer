#include <QTextCodec>
#include <QSettings>
#include <QByteArray>
#include <QBuffer>
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
#include <taglib/id3v2framefactory.h>
#include "mpegmetadatamodel.h"

MPEGMetaDataModel::MPEGMetaDataModel(bool using_rusxmms, const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable)
{
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);
    m_file = new TagLib::MPEG::File(m_stream, TagLib::ID3v2::FrameFactory::instance());
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::ID3v1);
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::ID3v2);
    m_tags << new MpegFileTagModel(using_rusxmms, m_file, TagLib::MPEG::File::APE);
}

MPEGMetaDataModel::~MPEGMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
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

    switch (ap->channelMode())
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

    return ep;
}

QList<TagModel* > MPEGMetaDataModel::tags() const
{
    return m_tags;
}

QPixmap MPEGMetaDataModel::cover() const
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

void MPEGMetaDataModel::setCover(const QPixmap &pix)
{
    TagLib::ID3v2::Tag *tag = m_file->ID3v2Tag(true);
    tag->removeFrames("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;
    frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);

    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    pix.save(&buffer, "JPEG");
    frame->setMimeType("image/jpeg");
    frame->setDescription("TTK");
    frame->setPicture(TagLib::ByteVector(data.constData(), data.size()));
    tag->addFrame(frame);
    m_file->save(TagLib::MPEG::File::ID3v2);
}

void MPEGMetaDataModel::removeCover()
{
    if(m_file->ID3v2Tag())
    {
        m_file->ID3v2Tag()->removeFrames("APIC");
        m_file->save(TagLib::MPEG::File::ID3v2);
    }
}

MpegFileTagModel::MpegFileTagModel(bool using_rusxmms, TagLib::MPEG::File *file, TagLib::MPEG::File::TagTypes tagType)
    : TagModel(),
      m_using_rusxmms(using_rusxmms),
      m_file(file),
      m_tagType(tagType)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MPEG");
    if(m_tagType == TagLib::MPEG::File::ID3v1)
    {
        m_tag = m_file->ID3v1Tag();
        m_codec = QTextCodec::codecForName(settings.value("ID3v1_encoding", "ISO-8859-1").toByteArray());
        if(!m_codec)
            QTextCodec::codecForName ("ISO-8859-1");
    }
    else if(m_tagType == TagLib::MPEG::File::ID3v2)
    {
        m_tag = m_file->ID3v2Tag();
        m_codec = QTextCodec::codecForName(settings.value("ID3v2_encoding", "UTF-8").toByteArray());
        if(!m_codec)
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
{

}

QString MpegFileTagModel::name() const
{
    if(m_tagType == TagLib::MPEG::File::ID3v1)
        return "ID3v1";
    else if(m_tagType == TagLib::MPEG::File::ID3v2)
        return "ID3v2";
    return "APE";
}

QList<Qmmp::MetaData> MpegFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    if(m_tagType == TagLib::MPEG::File::ID3v2)
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

QString MpegFileTagModel::value(Qmmp::MetaData key) const
{
    QTextCodec *codec = m_codec;

    if(m_tag)
    {
        bool utf = codec->name().contains("UTF");
        if(utf)
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

    if(m_tagType == TagLib::MPEG::File::ID3v1)
    {
        if(m_codec->name().contains("UTF") && !m_using_rusxmms) //utf is unsupported
            return;

        if(m_using_rusxmms)
            type = TagLib::String::UTF8;
    }
    else if(m_tagType == TagLib::MPEG::File::ID3v2)
    {
        if(m_codec->name().contains("UTF"))
        {
            type = TagLib::String::UTF8;
            if(m_codec->name().contains("UTF-16"))
                type = TagLib::String::UTF16;
            else if(m_codec->name().contains("UTF-16LE"))
                type = TagLib::String::UTF16LE;
            else if(m_codec->name().contains("UTF-16BE"))
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
        else if(key == Qmmp::COMPOSER)
            id3v2_key = "TCOM"; //composer
        else if(key == Qmmp::DISCNUMBER)
            id3v2_key = "TPOS";  //disc number

        if(!id3v2_key.isEmpty())
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

bool MpegFileTagModel::exists() const
{
    return (m_tag != nullptr);
}

void MpegFileTagModel::create()
{
    if(m_tag)
        return;
    if(m_tagType == TagLib::MPEG::File::ID3v1)
        m_tag = m_file->ID3v1Tag(true);
    else if(m_tagType == TagLib::MPEG::File::ID3v2)
        m_tag = m_file->ID3v2Tag(true);
    else if(m_tagType == TagLib::MPEG::File::APE)
        m_tag = m_file->APETag(true);
}

void MpegFileTagModel::remove()
{
    m_tag = nullptr;
}

void MpegFileTagModel::save()
{
    if(m_tag)
    {
        if(m_tagType == TagLib::MPEG::File::ID3v2)
        {
            m_file->save(m_tagType, false, 3);
        }
        else
            m_file->save(m_tagType, false);
    }
    else
        m_file->strip(m_tagType);
}
