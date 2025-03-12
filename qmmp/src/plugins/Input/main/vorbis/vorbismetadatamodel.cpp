#include "vorbismetadatamodel.h"

#include <QBuffer>

VorbisMetaDataModel::VorbisMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable),
      m_path(path)
{
    m_stream = new TagLib::FileStream(QStringToFileName(path), readOnly);
    m_file = new TagLib::Ogg::Vorbis::File(m_stream);
    m_tag = m_file->tag();
    m_tags << new VorbisCommentModel(this);
}

VorbisMetaDataModel::~VorbisMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_file;
    delete m_stream;
}

QList<TagModel*> VorbisMetaDataModel::tags() const
{
    return m_tags;
}

QImage VorbisMetaDataModel::cover() const
{
    if(!m_tag || m_tag->isEmpty())
        return QImage();

    TagLib::List<TagLib::FLAC::Picture *> list = m_tag->pictureList();
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

void VorbisMetaDataModel::setCover(const QImage &img)
{
    removeCover();
    if(m_tag)
    {
        TagLib::FLAC::Picture *picture = new TagLib::FLAC::Picture();
        picture->setType(TagLib::FLAC::Picture::FrontCover);

        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "JPEG");
        picture->setMimeType("image/jpeg");
        picture->setDescription("TTK");
        picture->setData(TagLib::ByteVector(data.constData(), data.length()));
        m_tag->addPicture(picture);
        m_file->save();
    }
}

void VorbisMetaDataModel::removeCover()
{
    if(m_tag && !m_tag->isEmpty())
    {
        bool save = false;
        TagLib::List<TagLib::FLAC::Picture *> list = m_tag->pictureList();
        for(uint i = 0; i < list.size(); ++i)
        {
            if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
            {
                m_tag->removePicture(list[i], false);
                save = true;
            }
        }

        if(save)
        {
            m_file->save();
        }
    }
}

QString VorbisMetaDataModel::lyrics() const
{
    if(m_tag && !m_tag->isEmpty())
    {
        const TagLib::Ogg::FieldListMap &items = m_tag->fieldListMap();

        if(!items["UNSYNCEDLYRICS"].isEmpty())
            return TStringToQString(items["UNSYNCEDLYRICS"].front());
        else if(!items["LYRICS"].isEmpty())
            return TStringToQString(items["LYRICS"].front());
    }
    return QString();
}


VorbisCommentModel::VorbisCommentModel(VorbisMetaDataModel *model)
    : TagModel(TagModel::Save),
      m_model(model)
{

}

QString VorbisCommentModel::name() const
{
    return "Vorbis Comment";
}

QString VorbisCommentModel::value(Qmmp::MetaData key) const
{
    if(!m_model->m_tag || m_model->m_tag->isEmpty())
        return QString();

    TagLib::Ogg::XiphComment *tag = m_model->m_tag;
    switch((int) key)
    {
    case Qmmp::TITLE:
        return TStringToQString(tag->title());
    case Qmmp::ARTIST:
        return TStringToQString(tag->artist());
    case Qmmp::ALBUMARTIST:
        if(tag->fieldListMap()["ALBUMARTIST"].isEmpty())
            return QString();
        else
            return TStringToQString(tag->fieldListMap()["ALBUMARTIST"].toString());
    case Qmmp::ALBUM:
        return TStringToQString(tag->album());
    case Qmmp::COMMENT:
        return TStringToQString(tag->comment());
    case Qmmp::GENRE:
        return TStringToQString(tag->genre());
    case Qmmp::COMPOSER:
        if(tag->fieldListMap()["COMPOSER"].isEmpty())
            return QString();
        else
            return TStringToQString(tag->fieldListMap()["COMPOSER"].toString());
    case Qmmp::YEAR:
        return QString::number(tag->year());
    case Qmmp::TRACK:
        return QString::number(tag->track());
    case Qmmp::DISCNUMBER:
        if(tag->fieldListMap()["DISCNUMBER"].isEmpty())
            return QString();
        else
            return TStringToQString(tag->fieldListMap()["DISCNUMBER"].toString());
    }
    return QString();
}

void VorbisCommentModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_model->m_tag || m_model->m_tag->isEmpty())
        return;

    TagLib::Ogg::XiphComment *tag = m_model->m_tag;

    TagLib::String str = QStringToTString(value);

    switch((int) key)
    {
    case Qmmp::TITLE:
        tag->setTitle(str);
        return;
    case Qmmp::ARTIST:
        tag->setArtist(str);
        return;
    case Qmmp::ALBUM:
        tag->setAlbum(str);
        return;
    case Qmmp::ALBUMARTIST:
        tag->addField("ALBUMARTIST", str, true);
        return;
    case Qmmp::COMMENT:
        tag->setComment(str);
        return;
    case Qmmp::GENRE:
        tag->setGenre(str);
        return;
    case Qmmp::COMPOSER:
        tag->addField("COMPOSER", str, true);
        return;
    case Qmmp::TRACK:
        tag->setTrack(value.toInt());
        return;
    case Qmmp::YEAR:
        tag->setYear(value.toInt());
        return;
    case Qmmp::DISCNUMBER:
        value == "0" ?
        tag->removeFields("DISCNUMBER"):
        tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_model->m_tag)
        m_model->m_file->save();
}
