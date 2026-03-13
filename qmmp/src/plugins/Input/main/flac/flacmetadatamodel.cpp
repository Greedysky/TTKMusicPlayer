#include <qmmp/metadatamanager.h>
#include "flacmetadatamodel.h"

#include <QBuffer>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2framefactory.h>
#include <taglib/textidentificationframe.h>

FLACMetaDataModel::FLACMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable)
{
    m_path = path.contains("://") ? TrackInfo::pathFromUrl(path) : path;

    if(m_path.endsWith(".flac", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
#if TAGLIB_MAJOR_VERSION >= 2
        m_nativeFlacFile = new TagLib::FLAC::File(m_stream);
#else
        m_nativeFlacFile = new TagLib::FLAC::File(m_stream, TagLib::ID3v2::FrameFactory::instance());
#endif
        m_tag = m_nativeFlacFile->xiphComment();

        if(m_nativeFlacFile->isValid())
        {
            m_tags << new VorbisCommentModel(m_nativeFlacFile);
            m_tags << new ID3v2TagModel(m_nativeFlacFile);
            setDialogHints(dialogHints() | MetaDataModel::IsCueEditable);
            setReadOnly(m_nativeFlacFile->readOnly());
        }
    }
    else if(m_path.endsWith(".oga", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
        m_oggFlacFile = new TagLib::Ogg::FLAC::File(m_stream);
        m_tag = m_oggFlacFile->tag();

        if(m_oggFlacFile->isValid())
        {
            m_tags << new VorbisCommentModel(m_oggFlacFile);
            setDialogHints(dialogHints() | MetaDataModel::IsCueEditable);
            setReadOnly(m_oggFlacFile->readOnly());
        }
    }
}

FLACMetaDataModel::~FLACMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_nativeFlacFile;
    delete m_oggFlacFile;
    delete m_stream;
}

QList<TagModel*> FLACMetaDataModel::tags() const
{
    return m_tags;
}

QImage FLACMetaDataModel::cover() const
{
    TagLib::List<TagLib::FLAC::Picture *> list;

    if(m_nativeFlacFile)
    {
        list = m_nativeFlacFile->pictureList(); //native flac
    }
    else if(m_tag)
    {
        list = m_tag->pictureList(); //ogg flac
    }

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

QString FLACMetaDataModel::coverPath() const
{
    return MetaDataManager::instance()->findCoverFile(m_path);
}

void FLACMetaDataModel::setCover(const QImage &img)
{
    removeCover();

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
    picture->setData(TagLib::ByteVector(data.constData(), data.size()));

    if(m_nativeFlacFile)
    {
        m_nativeFlacFile->addPicture(picture);
        m_nativeFlacFile->save();
    }
    else if(m_oggFlacFile && m_tag)
    {
        m_tag->addPicture(picture);
        m_oggFlacFile->save();
    }
}

void FLACMetaDataModel::removeCover()
{
    if(m_nativeFlacFile)
    {
        bool save = false;
        const TagLib::List<TagLib::FLAC::Picture *> list = m_nativeFlacFile->pictureList(); //native flac
        for(TagLib::FLAC::Picture *p : qAsConst(list))
        {
            if(p->type() == TagLib::FLAC::Picture::FrontCover)
            {
                m_nativeFlacFile->removePicture(p, false);
                save = true;
            }
        }

        if(save)
        {
            m_nativeFlacFile->save();
        }
    }
    else if(m_oggFlacFile && m_tag && !m_tag->isEmpty())
    {
        bool save = false;
        const TagLib::List<TagLib::FLAC::Picture *> list = m_tag->pictureList(); //ogg flac
        for(TagLib::FLAC::Picture *p : qAsConst(list))
        {
            if(p->type() == TagLib::FLAC::Picture::FrontCover)
            {
                m_tag->removePicture(p, false);
                save = true;
            }
        }

        if(save)
        {
            m_oggFlacFile->save();
        }
    }
}

QString FLACMetaDataModel::cue() const
{
    if(m_tag && m_tag->fieldListMap().contains("CUESHEET"))
    {
        return QString::fromUtf8(m_tag->fieldListMap()["CUESHEET"].toString().toCString(true));
    }
    return QString();
}

void FLACMetaDataModel::setCue(const QString &content)
{
    if(!m_tag && m_nativeFlacFile)
    {
        m_tag = m_nativeFlacFile->xiphComment(true);
    }

    if(m_tag)
    {
        m_tag->addField("CUESHEET", QStringToTString(content), true);
    }

    if(m_nativeFlacFile)
    {
        m_nativeFlacFile->save();
    }
    else if(m_oggFlacFile)
    {
        m_oggFlacFile->save();
    }
}

void FLACMetaDataModel::removeCue()
{
    if(m_tag)
    {
        m_tag->removeFields("CUESHEET");
        if(m_nativeFlacFile)
        {
            m_nativeFlacFile->save();
        }
        else if(m_oggFlacFile)
        {
            m_oggFlacFile->save();
        }
    }
}

QString FLACMetaDataModel::lyrics() const
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


VorbisCommentModel::VorbisCommentModel(TagLib::FLAC::File *file)
    : TagModel(TagModel::Save),
      m_nativeFlacFile(file),
      m_tag(file->xiphComment())
{

}

VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::FLAC::File *file)
    : TagModel(TagModel::Save),
      m_oggFlacFile(file),
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
    {
        return QString();
    }

    switch(key)
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
    if(!m_tag && m_nativeFlacFile)
    {
        m_tag = m_nativeFlacFile->xiphComment(true);
    }

    if(!m_tag)
    {
        return;
    }

    TagLib::String str = QStringToTString(value);

    switch(key)
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
    if(m_nativeFlacFile)
    {
         m_nativeFlacFile->save();
    }
    else if(m_oggFlacFile)
    {
         m_oggFlacFile->save();
    }
}


ID3v2TagModel::ID3v2TagModel(TagLib::FLAC::File *file)
    : TagModel(),
      m_file(file),
      m_tag(file->ID3v2Tag())
{

}

QString ID3v2TagModel::name() const
{
    return "ID3v2";
}

QString ID3v2TagModel::value(Qmmp::MetaData key) const
{
    if(!m_tag)
    {
        return QString();
    }

    switch(key)
    {
    case Qmmp::TITLE:
        return TStringToQString(m_tag->title());
    case Qmmp::ARTIST:
        return TStringToQString(m_tag->artist());
    case Qmmp::ALBUMARTIST:
        if(m_tag->frameListMap()["TPE2"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->frameListMap()["TPE2"].front()->toString());
    case Qmmp::ALBUM:
        return TStringToQString(m_tag->album());
    case Qmmp::COMMENT:
        return TStringToQString(m_tag->comment());
    case Qmmp::GENRE:
        return TStringToQString(m_tag->genre());
    case Qmmp::COMPOSER:
        if(m_tag->frameListMap()["TCOM"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->frameListMap()["TCOM"].front()->toString());
    case Qmmp::YEAR:
        return QString::number(m_tag->year());
    case Qmmp::TRACK:
        return QString::number(m_tag->track());
    case  Qmmp::DISCNUMBER:
        if(m_tag->frameListMap()["TPOS"].isEmpty())
            return QString();
        else
            return TStringToQString(m_tag->frameListMap()["TPOS"].front()->toString());
    case Qmmp::UNKNOWN:
        break;
    }
    return QString();
}

void ID3v2TagModel::setValue(Qmmp::MetaData key, const QString &value)
{
    if(!m_tag)
    {
        return;
    }

    TagLib::String str = QStringToTString(value);
    TagLib::ID3v2::FrameFactory::instance()->setDefaultTextEncoding(TagLib::String::UTF8);

    //save additional tags
    TagLib::ByteVector id3v2;
    if(key == Qmmp::ALBUMARTIST)
        id3v2 = "TPE2"; //album artist
    else if(key == Qmmp::COMPOSER)
        id3v2 = "TCOM"; //composer
    else if(key == Qmmp::DISCNUMBER)
        id3v2 = "TPOS";  //disc number

    if(!id3v2.isEmpty())
    {
        if(value.isEmpty())
            m_tag->removeFrames(id3v2);
        else if(!m_tag->frameListMap()[id3v2].isEmpty())
            m_tag->frameListMap()[id3v2].front()->setText(str);
        else
        {
            TagLib::ID3v2::TextIdentificationFrame *frame;
            frame = new TagLib::ID3v2::TextIdentificationFrame(id3v2, TagLib::String::UTF8);
            frame->setText(str);
            m_tag->addFrame(frame);
        }
        return;
    }

    switch(key)
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
    default:
        break;
    }
}

bool ID3v2TagModel::exists() const
{
    return m_tag != nullptr;
}

void ID3v2TagModel::create()
{
    m_tag = m_file->ID3v2Tag(true);
}

void ID3v2TagModel::remove()
{
    m_tag = nullptr;
}

void ID3v2TagModel::save()
{
    if(!m_tag)
        m_file->strip(TagLib::FLAC::File::ID3v2);
    m_file->save();
}
