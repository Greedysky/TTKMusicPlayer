#include "musicsongtag.h"
#include "musictime.h"
#include "musicformats.h"
#include "ttkversion.h"
#include "musicqmmputils.h"
#include "musicwidgetutils.h"
#include "musicstringutils.h"

#include <QStringList>
#include <QPluginLoader>
#include <QFileInfo>
///qmmp incldue
#include "tagwrapper.h"
#include "decoderfactory.h"
#include "metadatamodel.h"
#include "decoder.h"

MusicSongTag::MusicSongTag()
{

}

MusicSongTag::MusicSongTag(const QString &file)
    : MusicSongTag()
{
    m_filePath = file;
}

bool MusicSongTag::read()
{
    if(m_filePath.isEmpty())
    {
        return false;
    }

    return read(m_filePath);
}

bool MusicSongTag::read(const QString &file)
{
    const QFile f(file);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    m_filePath = file;
    return readOtherTaglib();
}

bool MusicSongTag::save()
{
    return saveOtherTaglib();
}

QString MusicSongTag::getDecoder() const
{
    const QString &v = findPluginPath();
    return QFileInfo(v).baseName();
}

QString MusicSongTag::getFilePath() const
{
    return m_filePath;
}

QString MusicSongTag::getArtist() const
{
    return findLegalDataString(TagWrapper::TAG_ARTIST);
}

QString MusicSongTag::getTitle() const
{
    return findLegalDataString(TagWrapper::TAG_TITLE);
}

QString MusicSongTag::getAlbum() const
{
    return findLegalDataString(TagWrapper::TAG_ALBUM);
}

QString MusicSongTag::getComment() const
{
    return m_parameters[TagWrapper::TAG_COMMENT].toString();
}

QString MusicSongTag::getYear() const
{
    return m_parameters[TagWrapper::TAG_YEAR].toString();
}

QString MusicSongTag::getTrackNum() const
{
    const QString &v = m_parameters[TagWrapper::TAG_TRACK].toString();
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? "-" : v;
    }
    return "-";
}

QString MusicSongTag::getGenre() const
{
    return findLegalDataString(TagWrapper::TAG_GENRE);
}

QString MusicSongTag::getAlbumArtist() const
{
    return findLegalDataString(TagWrapper::TAG_ALBUMARTIST);
}

QString MusicSongTag::getComposer() const
{
    return findLegalDataString(TagWrapper::TAG_COMPOSER);
}

QString MusicSongTag::getChannel() const
{
    return m_parameters[TagWrapper::TAG_CHANNEL].toString();
}

QString MusicSongTag::getURL() const
{
    return findLegalDataString(TagWrapper::TAG_URL);
}

void MusicSongTag::setArtist(const QString &artist)
{
    m_parameters[TagWrapper::TAG_ARTIST] = artist;
}

void MusicSongTag::setTitle(const QString &title)
{
    m_parameters[TagWrapper::TAG_TITLE] = title;
}

void MusicSongTag::setAlbum(const QString &album)
{
    m_parameters[TagWrapper::TAG_ALBUM] = album;
}

void MusicSongTag::setComment(const QString &comment)
{
    m_parameters[TagWrapper::TAG_COMMENT] = comment;
}

void MusicSongTag::setYear(const QString &year)
{
    m_parameters[TagWrapper::TAG_YEAR] = year;
}

void MusicSongTag::setTrackNum(const QString &track)
{
    m_parameters[TagWrapper::TAG_TRACK] = track;
}

void MusicSongTag::setGenre(const QString &genre)
{
    m_parameters[TagWrapper::TAG_GENRE] = genre;
}

void MusicSongTag::setCover(const QPixmap &pix)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap p(pix);
    if(p.width() > 500 || p.height() > 500)
    {
        p = p.scaled(500, 500, Qt::KeepAspectRatio);
    }
    m_parameters[TagWrapper::TAG_COVER] = p;
#else
    Q_UNUSED(data);
#endif
}

void MusicSongTag::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    setCover(pix);
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongTag::getCover() const
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    return m_parameters[TagWrapper::TAG_COVER].value<QPixmap>();
#else
    return QPixmap();
#endif
}

QString MusicSongTag::getSampleRate() const
{
    return m_parameters[TagWrapper::TAG_SAMPLERATE].toString();
}

QString MusicSongTag::getFormat() const
{
    return m_parameters[TagWrapper::TAG_FORMAT].toString();
}

QString MusicSongTag::getMode() const
{
    return m_parameters[TagWrapper::TAG_MODE].toString();
}

QString MusicSongTag::getBitrate() const
{
    return m_parameters[TagWrapper::TAG_BITRATE].toString() + " kbps";
}

QString MusicSongTag::getLengthString() const
{
    return MusicTime::msecTime2LabelJustified(m_parameters[TagWrapper::TAG_LENGTH].toULongLong());
}

QString MusicSongTag::findLegalDataString(TagWrapper::TagType type) const
{
    const QString &v = m_parameters[type].toString();
    return MusicUtils::String::illegalCharactersReplaced(v);
}

QString MusicSongTag::findPluginPath() const
{
    const QString &suffix = QFileInfo(m_filePath).suffix().toLower();

    const TTKStringListMap formats(MusicFormats::supportFormatsStringMap());
    for(const QString &key : formats.keys())
    {
        if(formats.value(key).contains(suffix))
        {
            return MusicUtils::QMMP::pluginPath("Input", key);
        }
    }

    return QString();
}

bool MusicSongTag::readOtherTaglib()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    const QObject *obj = loader.instance();
    DecoderFactory *factory = nullptr;
    if(obj && (factory = TTKObject_cast(DecoderFactory*, obj)))
    {
        qint64 length = 0;
        MetaDataModel *model = factory->createMetaDataModel(m_filePath, true);
        if(model)
        {
            m_parameters.insert(TagWrapper::TAG_COVER, model->cover());
            delete model;
        }

        const QList<TrackInfo*> infos(factory->createPlayList(m_filePath, TrackInfo::AllParts, nullptr));
        if(!infos.isEmpty())
        {
            TrackInfo *info = infos.first();
            m_parameters[TagWrapper::TAG_SAMPLERATE] = info->value(Qmmp::SAMPLERATE);
            m_parameters[TagWrapper::TAG_BITRATE] = info->value(Qmmp::BITRATE);
            m_parameters[TagWrapper::TAG_CHANNEL] = info->value(Qmmp::CHANNELS);

            m_parameters[TagWrapper::TAG_TITLE] = info->value(Qmmp::TITLE);
            m_parameters[TagWrapper::TAG_ARTIST] = info->value(Qmmp::ARTIST);
            m_parameters[TagWrapper::TAG_ALBUM] = info->value(Qmmp::ALBUM);
            m_parameters[TagWrapper::TAG_YEAR] = info->value(Qmmp::YEAR);
            m_parameters[TagWrapper::TAG_COMMENT] = info->value(Qmmp::COMMENT);
            m_parameters[TagWrapper::TAG_TRACK] = info->value(Qmmp::TRACK);
            m_parameters[TagWrapper::TAG_GENRE] = info->value(Qmmp::GENRE);

            length = info->duration();
            if(length != 0)
            {
                m_parameters.insert(TagWrapper::TAG_LENGTH, QString::number(length));
            }
        }

        if(length == 0)
        {
            TagWrapper tag;
            if(tag.readFile(m_filePath))
            {
                const QMap<TagWrapper::TagType, QString> &data = tag.getMusicTags();
                length = data[TagWrapper::TAG_LENGTH].toLongLong();
            }
            m_parameters[TagWrapper::TAG_LENGTH] = QString::number(length);
        }

        loader.unload();
    }

    return !m_parameters.isEmpty();
}

bool MusicSongTag::saveOtherTaglib()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    bool status = false;
    const QObject *obj = loader.instance();
    DecoderFactory *factory = nullptr;
    if(obj && (factory = TTKObject_cast(DecoderFactory*, obj)))
    {
        status = true;
        MetaDataModel *model = factory->createMetaDataModel(m_filePath, false);
        if(model)
        {
            const QList<TagModel* > &tags = model->tags();
            if(!tags.isEmpty())
            {
                TagModel *tagModel = tags.first();
                if(tags.count() == 3)
                {
                    tagModel = tags[1]; //id3v2 mode tag
                }

                tagModel->setValue(Qmmp::ALBUM, getAlbum());
                tagModel->setValue(Qmmp::ARTIST, getArtist());
                tagModel->setValue(Qmmp::TITLE, getTitle());
                tagModel->setValue(Qmmp::YEAR, getYear());
                tagModel->setValue(Qmmp::GENRE, getGenre());
                tagModel->setValue(Qmmp::TRACK, getTrackNum());
                tagModel->save();
            }

            //
            const QPixmap &pix = getCover();
            if(!pix.isNull())
            {
                model->setCover(pix);
            }
            else
            {
                model->removeCover();
            }
        }
        delete model;
        loader.unload();
    }

    return status;
}
