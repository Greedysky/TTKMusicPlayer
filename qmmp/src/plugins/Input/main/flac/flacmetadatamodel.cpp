#include <qmmp/metadatamanager.h>
#include "flacmetadatamodel.h"

#include <QBuffer>
#include <taglib/id3v2framefactory.h>

FLACMetaDataModel::FLACMetaDataModel(const QString &path, bool readOnly)
    : MetaDataModel(readOnly, MetaDataModel::IsCoverEditable)
{
    m_path = path.contains("://") ? TrackInfo::pathFromUrl(path) : path;

    if(m_path.endsWith(".flac", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
#if TAGLIB_MAJOR_VERSION >= 2
        TagLib::FLAC::File *f = new TagLib::FLAC::File(&stream);
#else
        TagLib::FLAC::File *f = new TagLib::FLAC::File(m_stream, TagLib::ID3v2::FrameFactory::instance());
#endif
        m_tag = f->xiphComment();
        m_file = f;
        setDialogHints(dialogHints() | MetaDataModel::IsCueEditable);
    }
    else if(m_path.endsWith(".oga", Qt::CaseInsensitive))
    {
        m_stream = new TagLib::FileStream(QStringToFileName(m_path), readOnly);
        TagLib::Ogg::FLAC::File *f = new TagLib::Ogg::FLAC::File(m_stream);
        m_tag = f->tag();
        m_file = f;
    }

    if(m_file)
        setReadOnly(m_file->readOnly());

    if(m_file && m_file->isValid() && !path.startsWith("flac://"))
    {
        m_tags << new VorbisCommentModel(m_tag, m_file);
    }
}

FLACMetaDataModel::~FLACMetaDataModel()
{
    qDeleteAll(m_tags);
    delete m_file;
    m_file = nullptr;
    delete m_stream;
}

QList<TagModel*> FLACMetaDataModel::tags() const
{
    return m_tags;
}

QImage FLACMetaDataModel::cover() const
{
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File *>(m_file);
    TagLib::List<TagLib::FLAC::Picture *> list;

    if(flacFile)
    {
        list = flacFile->pictureList(); //native flac
    }
    else if(m_tag && !m_tag->isEmpty())
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

    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File *>(m_file);
    TagLib::FLAC::Picture *picture = new TagLib::FLAC::Picture();
    picture->setType(TagLib::FLAC::Picture::FrontCover);

    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPEG");
    picture->setMimeType("image/jpeg");
    picture->setDescription("TTK");
    picture->setData(TagLib::ByteVector(data.constData(), data.size()));

    if(flacFile)
    {
        flacFile->addPicture(picture);
        flacFile->save();
    }
    else if(m_tag && m_file)
    {
        m_tag->addPicture(picture);
        m_file->save();
    }
}

void FLACMetaDataModel::removeCover()
{
    TagLib::FLAC::File *flacFile = dynamic_cast<TagLib::FLAC::File *>(m_file);
    bool save = false;

    if(flacFile)
    {
        const TagLib::List<TagLib::FLAC::Picture *> list = flacFile->pictureList(); //native flac
        for(TagLib::FLAC::Picture *p : qAsConst(list))
        {
            if(p->type() == TagLib::FLAC::Picture::FrontCover)
            {
                flacFile->removePicture(p, false);
                save = true;
            }
        }
    }
    else if(m_tag && !m_tag->isEmpty())
    {
        const TagLib::List<TagLib::FLAC::Picture *> list = m_tag->pictureList(); //ogg flac
        for(TagLib::FLAC::Picture *p : qAsConst(list))
        {
            if(p->type() == TagLib::FLAC::Picture::FrontCover)
            {
                m_tag->removePicture(p, false);
                save = true;
            }
        }
    }

    if(save)
    {
        m_file->save();
    }
}

QString FLACMetaDataModel::cue() const
{
    if(m_tag->fieldListMap().contains("CUESHEET"))
    {
        return QString::fromUtf8(m_tag->fieldListMap()["CUESHEET"].toString().toCString(true));
    }

    return QString();
}

void FLACMetaDataModel::setCue(const QString &content)
{
    m_tag->removeFields("CUESHEET");
    m_tag->addField("CUESHEET", QStringToTString(content), true);
    m_file->save();
}

void FLACMetaDataModel::removeCue()
{
    m_tag->removeFields("CUESHEET");
    m_file->save();
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


VorbisCommentModel::VorbisCommentModel(TagLib::Ogg::XiphComment *tag, TagLib::File *file)
    : TagModel(TagModel::Save),
      m_file(file),
      m_tag(tag)
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
        m_tag->removeFields("DISCNUMBER"):
        m_tag->addField("DISCNUMBER", str, true);
    }
}

void VorbisCommentModel::save()
{
    if(m_file)
        m_file->save();
}
