#include <QBuffer>
#include <taglib/fileref.h>
#include "opusmetadatamodel.h"

OpusMetaDataModel::OpusMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable),
      m_path(path)
{
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);
    m_file = new TagLib::Ogg::Opus::File(m_stream);
    m_tags << new VorbisCommentModel(m_file);
}

OpusMetaDataModel::~OpusMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_file;
    delete m_stream;
}

QList<MetaDataItem> OpusMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    TagLib::Ogg::Opus::Properties *ap = m_file->audioProperties();
    if(ap)
    {
        ep << MetaDataItem(tr("Version"), ap->opusVersion());
    }
    return ep;
}

QList<TagModel*> OpusMetaDataModel::tags() const
{
    return m_tags;
}

QImage OpusMetaDataModel::cover() const
{
    if(!m_file || !m_file->isValid())
        return QImage();

    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(!tag || tag->isEmpty())
        return QImage();

    TagLib::List<TagLib::FLAC::Picture *> list = tag->pictureList();
    for(uint i = 0; i < list.size(); ++i)
    {
        if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
        {
            QImage cover;
            cover.loadFromData(QByteArray(list[i]->data().data(), list[i]->data().size())); //read binary picture data
            return cover;
        }
    }
    return QImage();
}

void OpusMetaDataModel::setCover(const QImage &img)
{
    removeCover();
    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(tag)
    {
        TagLib::FLAC::Picture *picture = new TagLib::FLAC::Picture();
        picture->setType(TagLib::FLAC::Picture::FrontCover);

        QByteArray data;
        QBuffer buffer(&data);
        if(buffer.open(QIODevice::WriteOnly))
        {
            img.save(&buffer, "JPEG");
        }
        picture->setMimeType("image/jpeg");
        picture->setDescription("TTK");
        picture->setData(TagLib::ByteVector(data.constData(), data.length()));
        tag->addPicture(picture);
        m_file->save();
    }
}

void OpusMetaDataModel::removeCover()
{
    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(tag && !tag->isEmpty())
    {
        bool save = false;
        TagLib::List<TagLib::FLAC::Picture *> list = tag->pictureList();
        for(uint i = 0; i < list.size(); ++i)
        {
            if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
            {
                tag->removePicture(list[i], false);
                save = true;
            }
        }

        if(save)
        {
            m_file->save();
        }
    }
}

QString OpusMetaDataModel::lyrics() const
{
    TagLib::Ogg::XiphComment *tag = m_file->tag();
    if(tag && !tag->isEmpty())
    {
        const TagLib::Ogg::FieldListMap &items = tag->fieldListMap();

        if(!items["UNSYNCEDLYRICS"].isEmpty())
            return TStringToQString(items["UNSYNCEDLYRICS"].front());
        else if(!items["LYRICS"].isEmpty())
            return TStringToQString(items["LYRICS"].front());
    }
    return QString();
}


VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::Opus::File *file)
    : TagModel(TagModel::Save),
      m_file(file),
      m_tag(file->tag())
{

}

QString VorbisCommentModel::name() const
{
    return "Vorbis Comment";
}

QString VorbisCommentModel::value(Qmmp::MetaData key) const
{
    if(!m_tag)
        return QString();
    switch((int) key)
    {
    case Qmmp::TITLE:
        return TStringToQString(m_tag->title());
    case Qmmp::ARTIST:
        return TStringToQString(m_tag->artist());
    case Qmmp::ALBUMARTIST:
        if(m_tag->fieldListMap()["ALBUMARTIST"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["ALBUMARTIST"].toString());
    case Qmmp::ALBUM:
        return TStringToQString(m_tag->album());
    case Qmmp::COMMENT:
        return TStringToQString(m_tag->comment());
    case Qmmp::GENRE:
        return TStringToQString(m_tag->genre());
    case Qmmp::COMPOSER:
        if(m_tag->fieldListMap()["COMPOSER"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["COMPOSER"].toString());
    case Qmmp::YEAR:
        return QString::number(m_tag->year());
    case Qmmp::TRACK:
        return QString::number(m_tag->track());
    case Qmmp::DISCNUMBER:
        if(m_tag->fieldListMap()["DISCNUMBER"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->fieldListMap()["DISCNUMBER"].toString());
    }
    return QString();
}

void VorbisCommentModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
        return;

    TagLib::String str = QStringToTString(value);

    switch((int) key)
    {
    case Qmmp::TITLE:
        m_tag->setTitle(str);
        return;
    case Qmmp::ARTIST:
        m_tag->setArtist(str);
        return;
    case Qmmp::ALBUMARTIST:
        m_tag->addField("ALBUMARTIST", str, true);
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
    case Qmmp::COMPOSER:
        m_tag->addField("COMPOSER", str, true);
        return;
    case Qmmp::TRACK:
        m_tag->setTrack(value.toInt());
        return;
    case Qmmp::YEAR:
        m_tag->setYear(value.toInt());
        return;
    case Qmmp::DISCNUMBER:
        value == "0" ?
        m_tag->removeFields("DISCNUMBER") :
        m_tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_file)
        m_file->save();
}
