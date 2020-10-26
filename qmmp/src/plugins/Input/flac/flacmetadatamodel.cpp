#include <QPixmap>
#include <QBuffer>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tmap.h>
#include <taglib/id3v2framefactory.h>
#include <taglib/flacpicture.h>
#ifndef HAS_PICTURE_LIST
#include <FLAC/all.h>
#endif
#include <qmmp/metadatamanager.h>
#include "flacmetadatamodel.h"

FLACMetaDataModel::FLACMetaDataModel(const QString &path, bool readOnly)
#ifdef HAS_PICTURE_LIST
    : MetaDataModel(true, MetaDataModel::IsCoverEditable),
#else
    : MetaDataModel(true),
#endif
      m_path(path)
{
    if(path.startsWith("flac://"))
    {
        m_path.remove("flac://");
        m_path.remove(RegularWrapper("#\\d+$"));
    }

    if(m_path.endsWith(".flac", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
        TagLib::FLAC::File *f = new TagLib::FLAC::File(m_stream, TagLib::ID3v2::FrameFactory::instance());
        m_tag = f->xiphComment();
        m_file = f;
    }
    else if(m_path.endsWith(".oga", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
        TagLib::Ogg::FLAC::File *f = new TagLib::Ogg::FLAC::File(m_stream);
        m_tag = f->tag();
        m_file = f;
    }

    if(m_file && m_file->isValid() && !path.startsWith("flac://"))
    {
        setReadOnly(readOnly);
        m_tags << new VorbisCommentModel(m_tag, m_file);
    }
}

FLACMetaDataModel::~FLACMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    if(m_file)
    {
        delete m_file;
        m_file = nullptr;
    }
    if(m_stream)
        delete m_stream;
}

QList<TagModel* > FLACMetaDataModel::tags() const
{
    return m_tags;
}

QPixmap FLACMetaDataModel::cover() const
{
#ifdef HAS_PICTURE_LIST
    if(!m_tag || m_tag->isEmpty())
        return QPixmap();

    TagLib::List<TagLib::FLAC::Picture *> list = m_tag->pictureList();
    for(uint i = 0; i < list.size(); ++i)
    {
        if(list[i]->type() == TagLib::FLAC::Picture::FrontCover)
        {
            QPixmap cover;
            cover.loadFromData(QByteArray(list[i]->data().data(), list[i]->data().size())); //read binary picture data
            return cover;
        }
    }
    return QPixmap();
#else
    //embedded cover
     QPixmap cover;
     FLAC__StreamMetadata *metadata;
     FLAC__metadata_get_picture(qPrintable(m_path),
                                &metadata,
                                FLAC__STREAM_METADATA_PICTURE_TYPE_FRONT_COVER,
                                0,0, -1,-1,-1,-1);
     if(metadata)
     {
         FLAC__StreamMetadata_Picture *pict = &metadata->data.picture;
         cover.loadFromData(QByteArray((char *)pict->data, (int) pict->data_length));
         FLAC__metadata_object_delete(metadata);
     }
     return cover;
#endif
}

QString FLACMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}

#ifdef HAS_PICTURE_LIST
void FLACMetaDataModel::setCover(const QPixmap &pix)
{
    removeCover();
    if(m_tag && !m_tag->isEmpty())
    {
        TagLib::FLAC::Picture *picture = new TagLib::FLAC::Picture();
        picture->setType(TagLib::FLAC::Picture::FrontCover);

        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "JPEG");
        picture->setMimeType("image/jpeg");
        picture->setDescription("TTK");
        picture->setData(TagLib::ByteVector(data.constData(), data.size()));
        m_tag->addPicture(picture);
        m_file->save();
    }
}

void FLACMetaDataModel::removeCover()
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
#endif

VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::XiphComment *tag, TagLib::File *file)
    : TagModel(TagModel::Save),
      m_file(file),
      m_tag(tag)
{

}

VorbisCommentModel::~VorbisCommentModel()
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
    case  Qmmp::DISCNUMBER:
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
        m_tag->removeField("DISCNUMBER"):
        m_tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_file)
        m_file->save();
}
