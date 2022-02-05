#include "musicsongmeta.h"
#include "musictime.h"
#include "musicformats.h"
#include "musicqmmputils.h"
#include "musicstringutils.h"
#include "ttkversion.h"

#include <qmmp/decoderfactory.h>
#include <qmmp/metadatamodel.h>
#include <qmmp/decoder.h>

/*! @brief The class of the music meta.
 * @author Greedysky <greedysky@163.com>
 */
struct MusicMeta
{
    QPixmap m_cover;
    QString m_fileUrl;
    QMap<TagWrapper::Type, QString> m_metaData;
};


MusicSongMeta::MusicSongMeta()
    : m_offset(-1)
{

}

MusicSongMeta::~MusicSongMeta()
{
    clearSongMeta();
}

bool MusicSongMeta::read(const QString &file)
{
    bool track = false;
    QString path(file);
    if(MusicFormats::songTrackValid(file))
    {
        path = path.section("://", -1);
        if(path.contains("#"))
        {
            path = path.section("#", 0, 0);
            track = true;
        }
    }

    const QFile f(path);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    m_path = path;
    const bool status = readInformation();
    if(status && track)
    {
        setSongMetaIndex(file.section("#", -1).toInt() - 1);
    }

    return status;
}

bool MusicSongMeta::save()
{
    return saveInformation();
}

QString MusicSongMeta::decoder() const
{
    const QString &suffix = QFileInfo(m_path).suffix().toLower();
    const TTKStringListMap formats(MusicFormats::supportMusicMapFormats());
    for(const QString &key : formats.keys())
    {
        if(formats.value(key).contains(suffix))
        {
            return QFileInfo(MusicUtils::QMMP::pluginPath("Input", key)).baseName();
        }
    }

    return QString();
}

QString MusicSongMeta::filePath() const
{
    return m_path;
}

QString MusicSongMeta::fileBasePath()
{
    return songMeta()->m_fileUrl;
}

QString MusicSongMeta::fileRelatedPath()
{
    return songMeta()->m_metaData[TagWrapper::TAG_URL];
}

QString MusicSongMeta::artist()
{
    return findLegalDataString(TagWrapper::TAG_ARTIST);
}

QString MusicSongMeta::title()
{
    return findLegalDataString(TagWrapper::TAG_TITLE);
}

QString MusicSongMeta::album()
{
    return findLegalDataString(TagWrapper::TAG_ALBUM);
}

QString MusicSongMeta::comment()
{
    return songMeta()->m_metaData[TagWrapper::TAG_COMMENT];
}

QString MusicSongMeta::year()
{
    return songMeta()->m_metaData[TagWrapper::TAG_YEAR];
}

QString MusicSongMeta::trackNum()
{
    const QString &v = songMeta()->m_metaData[TagWrapper::TAG_TRACK];
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? TTK_DEFAULT_STR : v;
    }
    return TTK_DEFAULT_STR;
}

QString MusicSongMeta::genre()
{
    return findLegalDataString(TagWrapper::TAG_GENRE);
}

QString MusicSongMeta::channel()
{
    return songMeta()->m_metaData[TagWrapper::TAG_CHANNEL];
}

void MusicSongMeta::setArtist(const QString &artist)
{
    songMeta()->m_metaData[TagWrapper::TAG_ARTIST] = artist;
}

void MusicSongMeta::setTitle(const QString &title)
{
    songMeta()->m_metaData[TagWrapper::TAG_TITLE] = title;
}

void MusicSongMeta::setAlbum(const QString &album)
{
    songMeta()->m_metaData[TagWrapper::TAG_ALBUM] = album;
}

void MusicSongMeta::setComment(const QString &comment)
{
    songMeta()->m_metaData[TagWrapper::TAG_COMMENT] = comment;
}

void MusicSongMeta::setYear(const QString &year)
{
    songMeta()->m_metaData[TagWrapper::TAG_YEAR] = year;
}

void MusicSongMeta::setTrackNum(const QString &track)
{
    songMeta()->m_metaData[TagWrapper::TAG_TRACK] = track;
}

void MusicSongMeta::setGenre(const QString &genre)
{
    songMeta()->m_metaData[TagWrapper::TAG_GENRE] = genre;
}

void MusicSongMeta::setCover(const QPixmap &pix)
{
#if TTK_VERSION >= TTK_VERSION_CHECK(2,5,3,0)
    QPixmap p(pix);
    if(p.width() > 500 || p.height() > 500)
    {
        p = p.scaled(500, 500, Qt::KeepAspectRatio);
    }
    songMeta()->m_cover = p;
#else
    Q_UNUSED(pix);
#endif
}

void MusicSongMeta::setCover(const QByteArray &data)
{
    if(data.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
        return;
    }
#if TTK_VERSION >= TTK_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    setCover(pix);
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongMeta::cover()
{
#if TTK_VERSION >= TTK_VERSION_CHECK(2,5,3,0)
    return songMeta()->m_cover;
#else
    return QPixmap();
#endif
}

QString MusicSongMeta::sampleRate()
{
    return songMeta()->m_metaData[TagWrapper::TAG_SAMPLERATE];
}

QString MusicSongMeta::bitrate()
{
    const QString &bitrate = songMeta()->m_metaData[TagWrapper::TAG_BITRATE];
    return bitrate.isEmpty() ? TTK_DEFAULT_STR : bitrate + " kbps";
}

QString MusicSongMeta::lengthString()
{
    return songMeta()->m_metaData[TagWrapper::TAG_LENGTH];
}

MusicSongMeta::MusicSongMeta(const MusicSongMeta &other)
{
    if(this == &other)
    {
        return;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;
    for(const MusicMeta *meta : m_songMetas)
    {
        m_songMetas << new MusicMeta(*meta);
    }
}

MusicSongMeta::MusicSongMeta(MusicSongMeta &&other)
{
    if(this == &other)
    {
        return;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;
    m_songMetas = other.m_songMetas;
    other.m_songMetas.clear();
}

MusicSongMeta& MusicSongMeta::operator= (const MusicSongMeta &other)
{
    if(this == &other)
    {
        return *this;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;
    for(const MusicMeta *meta : m_songMetas)
    {
        m_songMetas << new MusicMeta(*meta);
    }

    return *this;
}

MusicSongMeta& MusicSongMeta::operator= (MusicSongMeta &&other)
{
    if(this == &other)
    {
        return *this;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;
    m_songMetas = other.m_songMetas;
    other.m_songMetas.clear();

    return *this;
}

void MusicSongMeta::setSongMetaIndex(int index)
{
    if(index < 0 || index >= m_songMetas.size())
    {
        return;
    }

    m_offset = index;
}

int MusicSongMeta::songMetaSize() const
{
    return m_songMetas.size();
}

void MusicSongMeta::clearSongMeta()
{
    qDeleteAll(m_songMetas);
    m_songMetas.clear();
    m_offset = -1;
}

MusicMeta *MusicSongMeta::songMeta()
{
    if(m_songMetas.isEmpty())
    {
        m_songMetas << new MusicMeta;
        m_offset = 0;
    }

    return m_songMetas[m_offset];
}

QString MusicSongMeta::findLegalDataString(TagWrapper::Type type)
{
    const QString &v = songMeta()->m_metaData[type];
    return MusicUtils::String::charactersReplaced(v);
}

bool MusicSongMeta::readInformation()
{
    clearSongMeta();

    DecoderFactory *factory = Decoder::findByFilePath(m_path);
    if(factory)
    {
        QPixmap cover;
        MetaDataModel *model = factory->createMetaDataModel(m_path, true);
        if(model)
        {
            cover = model->cover();
            delete model;
        }

        qint64 length = 0;
        QStringList files;
        const QList<TrackInfo*> infos(factory->createPlayList(m_path, TrackInfo::AllParts, &files));

        for(TrackInfo *info : qAsConst(infos))
        {
            MusicMeta *meta = new MusicMeta;
            meta->m_fileUrl = info->path();
            meta->m_metaData[TagWrapper::TAG_URL] = files.isEmpty() ? meta->m_fileUrl : files.first();

            meta->m_metaData[TagWrapper::TAG_SAMPLERATE] = info->value(Qmmp::SAMPLERATE);
            meta->m_metaData[TagWrapper::TAG_BITRATE] = info->value(Qmmp::BITRATE);
            meta->m_metaData[TagWrapper::TAG_CHANNEL] = info->value(Qmmp::CHANNELS);

            meta->m_metaData[TagWrapper::TAG_TITLE] = info->value(Qmmp::TITLE);
            meta->m_metaData[TagWrapper::TAG_ARTIST] = info->value(Qmmp::ARTIST);
            meta->m_metaData[TagWrapper::TAG_ALBUM] = info->value(Qmmp::ALBUM);
            meta->m_metaData[TagWrapper::TAG_YEAR] = info->value(Qmmp::YEAR);
            meta->m_metaData[TagWrapper::TAG_COMMENT] = info->value(Qmmp::COMMENT);
            meta->m_metaData[TagWrapper::TAG_TRACK] = info->value(Qmmp::TRACK);
            meta->m_metaData[TagWrapper::TAG_GENRE] = info->value(Qmmp::GENRE);

            length = info->duration();
            if(length != 0)
            {
                meta->m_metaData[TagWrapper::TAG_LENGTH] = MusicTime::msecTime2LabelJustified(length);
            }

            m_songMetas << meta;
            m_offset = 0;
        }
        qDeleteAll(infos);

        if(!m_songMetas.isEmpty())
        {
            songMeta()->m_cover = cover;

            if(length == 0)
            {
                TagWrapper wrapper;
                if(wrapper.readFile(m_path))
                {
                    const QMap<TagWrapper::Type, QString> &data = wrapper.musicTags();
                    length = data[TagWrapper::TAG_LENGTH].toLongLong();
                }
                songMeta()->m_metaData[TagWrapper::TAG_LENGTH] = MusicTime::msecTime2LabelJustified(length);
            }
        }
    }

    return !m_songMetas.isEmpty();
}

bool MusicSongMeta::saveInformation()
{
    DecoderFactory *factory = Decoder::findByFilePath(m_path);

    if(factory)
    {
        MetaDataModel *model = factory->createMetaDataModel(m_path, false);
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

                tagModel->setValue(Qmmp::ALBUM, album());
                tagModel->setValue(Qmmp::ARTIST, artist());
                tagModel->setValue(Qmmp::TITLE, title());
                tagModel->setValue(Qmmp::YEAR, year());
                tagModel->setValue(Qmmp::GENRE, genre());
                tagModel->setValue(Qmmp::TRACK, trackNum());
                tagModel->save();
            }

            const QPixmap &pix = cover();
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
        return true;
    }

    return false;
}
