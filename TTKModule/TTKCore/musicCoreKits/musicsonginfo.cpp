#include "musicsonginfo.h"
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

MusicSongInfo::MusicSongInfo()
{

}

MusicSongInfo::MusicSongInfo(const QString &file)
    : MusicSongInfo()
{
    m_filePath = file;
}

bool MusicSongInfo::read()
{
    if(m_filePath.isEmpty())
    {
        return false;
    }

    return read(m_filePath);
}

bool MusicSongInfo::read(const QString &file)
{
    const QFile f(file);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    m_filePath = file;
    return readInformation();
}

bool MusicSongInfo::save()
{
    return saveInformation();
}

QString MusicSongInfo::getDecoder() const
{
    const QString &v = findPluginPath();
    return QFileInfo(v).baseName();
}

QString MusicSongInfo::getFilePath() const
{
    return m_filePath;
}

QString MusicSongInfo::getArtist() const
{
    return findLegalDataString(TagWrapper::TAG_ARTIST);
}

QString MusicSongInfo::getTitle() const
{
    return findLegalDataString(TagWrapper::TAG_TITLE);
}

QString MusicSongInfo::getAlbum() const
{
    return findLegalDataString(TagWrapper::TAG_ALBUM);
}

QString MusicSongInfo::getComment() const
{
    return m_parameters[TagWrapper::TAG_COMMENT].toString();
}

QString MusicSongInfo::getYear() const
{
    return m_parameters[TagWrapper::TAG_YEAR].toString();
}

QString MusicSongInfo::getTrackNum() const
{
    const QString &v = m_parameters[TagWrapper::TAG_TRACK].toString();
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? STRING_NULL : v;
    }
    return STRING_NULL;
}

QString MusicSongInfo::getGenre() const
{
    return findLegalDataString(TagWrapper::TAG_GENRE);
}

QString MusicSongInfo::getAlbumArtist() const
{
    return findLegalDataString(TagWrapper::TAG_ALBUMARTIST);
}

QString MusicSongInfo::getComposer() const
{
    return findLegalDataString(TagWrapper::TAG_COMPOSER);
}

QString MusicSongInfo::getChannel() const
{
    return m_parameters[TagWrapper::TAG_CHANNEL].toString();
}

QString MusicSongInfo::getURL() const
{
    return findLegalDataString(TagWrapper::TAG_URL);
}

void MusicSongInfo::setArtist(const QString &artist)
{
    m_parameters[TagWrapper::TAG_ARTIST] = artist;
}

void MusicSongInfo::setTitle(const QString &title)
{
    m_parameters[TagWrapper::TAG_TITLE] = title;
}

void MusicSongInfo::setAlbum(const QString &album)
{
    m_parameters[TagWrapper::TAG_ALBUM] = album;
}

void MusicSongInfo::setComment(const QString &comment)
{
    m_parameters[TagWrapper::TAG_COMMENT] = comment;
}

void MusicSongInfo::setYear(const QString &year)
{
    m_parameters[TagWrapper::TAG_YEAR] = year;
}

void MusicSongInfo::setTrackNum(const QString &track)
{
    m_parameters[TagWrapper::TAG_TRACK] = track;
}

void MusicSongInfo::setGenre(const QString &genre)
{
    m_parameters[TagWrapper::TAG_GENRE] = genre;
}

void MusicSongInfo::setCover(const QPixmap &pix)
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

void MusicSongInfo::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    setCover(pix);
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongInfo::getCover() const
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    return m_parameters[TagWrapper::TAG_COVER].value<QPixmap>();
#else
    return QPixmap();
#endif
}

QString MusicSongInfo::getSampleRate() const
{
    return m_parameters[TagWrapper::TAG_SAMPLERATE].toString();
}

QString MusicSongInfo::getFormat() const
{
    return m_parameters[TagWrapper::TAG_FORMAT].toString();
}

QString MusicSongInfo::getMode() const
{
    return m_parameters[TagWrapper::TAG_MODE].toString();
}

QString MusicSongInfo::getBitrate() const
{
    return m_parameters[TagWrapper::TAG_BITRATE].toString() + " kbps";
}

QString MusicSongInfo::getLengthString() const
{
    return MusicTime::msecTime2LabelJustified(m_parameters[TagWrapper::TAG_LENGTH].toULongLong());
}

QString MusicSongInfo::findLegalDataString(TagWrapper::Type type) const
{
    const QString &v = m_parameters[type].toString();
    return MusicUtils::String::illegalCharactersReplaced(v);
}

QString MusicSongInfo::findPluginPath() const
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

bool MusicSongInfo::readInformation()
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
                const QMap<TagWrapper::Type, QString> &data = tag.getMusicTags();
                length = data[TagWrapper::TAG_LENGTH].toLongLong();
            }
            m_parameters[TagWrapper::TAG_LENGTH] = QString::number(length);
        }

        loader.unload();
    }

    return !m_parameters.isEmpty();
}

bool MusicSongInfo::saveInformation()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    const QObject *obj = loader.instance();
    DecoderFactory *factory = nullptr;
    if(obj && (factory = TTKObject_cast(DecoderFactory*, obj)))
    {
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
        return true;
    }

    return false;
}
