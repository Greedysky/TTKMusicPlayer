#include "ffapmetadatamodel.h"
#include <qmmp/qmmptextcodec.h>
#include <qmmp/metadatamanager.h>

#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>

FFapMetaDataModel::FFapMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(true)
{
    if(path.contains("://"))
    {
        m_path = TrackInfo::pathFromUrl(path);
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), true);
        m_file = new TagLib::APE::File(m_stream);
    }
    else
    {
        m_path = path;
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
        m_file = new TagLib::APE::File(m_stream);
        m_tags << new FFapFileTagModel(m_file, TagLib::APE::File::ID3v1);
        m_tags << new FFapFileTagModel(m_file, TagLib::APE::File::APE);
        setReadOnly(readOnly);
    }
}

FFapMetaDataModel::~FFapMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_file;
    delete m_stream;
}

QList<MetaDataItem> FFapMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    TagLib::APE::Properties *ap = m_file->audioProperties();
    if(ap)
    {
        ep << MetaDataItem(tr("Samples"), ap->sampleFrames());
        ep << MetaDataItem(tr("Version"), ap->version());
    }
    return ep;
}

QList<TagModel*> FFapMetaDataModel::tags() const
{
    return m_tags;
}

QString FFapMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}


FFapFileTagModel::FFapFileTagModel(TagLib::APE::File *file, TagLib::APE::File::TagTypes type)
    : TagModel(),
      m_file(file),
      m_type(type)
{
    if(m_type == TagLib::APE::File::ID3v1)
    {
        m_tag = m_file->ID3v1Tag();
        m_codec = new QmmpTextCodec("GB18030");
    }
    else
    {
        m_tag = m_file->APETag();
        m_codec = new QmmpTextCodec("UTF-8");
    }
}

QString FFapFileTagModel::name() const
{
    if(m_type == TagLib::APE::File::ID3v1)
        return "ID3v1";
    return "APE";
}

QList<Qmmp::MetaData> FFapFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::DISCNUMBER);
    return list;
}

QString FFapFileTagModel::value(Qmmp::MetaData key) const
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
        case Qmmp::ALBUM:
            str = m_tag->album();
            break;
        case Qmmp::COMMENT:
            str = m_tag->comment();
            break;
        case Qmmp::GENRE:
            str = m_tag->genre();
            break;
        case Qmmp::YEAR:
            return QString::number(m_tag->year());
        case Qmmp::TRACK:
            return QString::number(m_tag->track());
        }
        return CSTR_TO_QSTR(m_codec, str, utf);
    }
    return QString();
}

void FFapFileTagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;
    TagLib::String::Type type = TagLib::String::Latin1;

    if(m_type == TagLib::APE::File::ID3v1)
    {
        if(m_codec->name().contains("UTF")) //utf is unsupported
            return;
    }
    else if(m_type == TagLib::APE::File::APE)
        type = TagLib::String::UTF8;

    TagLib::String str = TagLib::String(m_codec->fromUnicode(value).constData(), type);

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

bool FFapFileTagModel::exists() const
{
    return (m_tag != nullptr);
}

void FFapFileTagModel::create()
{
    if(m_tag)
        return;
    if(m_type == TagLib::APE::File::ID3v1)
        m_tag = m_file->ID3v1Tag(true);
    else
        m_tag = m_file->APETag(true);
}

void FFapFileTagModel::remove()
{
    m_tag = nullptr;
}

void FFapFileTagModel::save()
{
    if(!m_tag)
        m_file->strip(m_type);
    m_file->save();
}
