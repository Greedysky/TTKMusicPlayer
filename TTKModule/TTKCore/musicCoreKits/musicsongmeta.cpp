#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicstringutils.h"
#include "ttktime.h"
#include "ttkversion.h"

#include <qmmp/decoder.h>
#include <qmmp/metadatamodel.h>
#include <qmmp/decoderfactory.h>

/*! @brief The class of the music meta.
 * @author Greedysky <greedysky@163.com>
 */
struct MusicMeta
{
    QString m_path;
    QPixmap m_cover;
    QString m_lyrics;
    QMap<TagMeta::Type, QString> m_metaData;
};


MusicSongMeta::MusicSongMeta() noexcept
    : m_offset(-1)
{

}

MusicSongMeta::~MusicSongMeta() noexcept
{
    clearSongMeta();
}

bool MusicSongMeta::read(const QString &url)
{
    bool track = false;
    QString path(url);
    // replace windows \\ path to / path
    path.replace(TTK_WSEPARATOR, TTK_SEPARATOR);

    if(MusicFormats::isTrack(url))
    {
        path = path.section("://", -1);
        if(path.contains("#"))
        {
            path = path.section("#", 0, 0);
            track = true;
        }
    }

    const QFile file(path);
    if(!file.exists() || file.size() <= 0)
    {
        return false;
    }

    m_path = path;
    if(!readInformation())
    {
        return false;
    }

    setSongMetaIndex(track ? url.section("#", -1).toInt() - 1 : 0);
    return true;
}

bool MusicSongMeta::save()
{
    return saveInformation();
}

QString MusicSongMeta::filePath() const noexcept
{
    return m_path;
}

QString MusicSongMeta::fileBasePath() noexcept
{
    return songMeta()->m_path;
}

QString MusicSongMeta::fileRelatedPath() noexcept
{
    return songMeta()->m_metaData[TagMeta::URL];
}

QString MusicSongMeta::artist() noexcept
{
    return formatString(TagMeta::ARTIST);
}

QString MusicSongMeta::title() noexcept
{
    return formatString(TagMeta::TITLE);
}

QString MusicSongMeta::album() noexcept
{
    return formatString(TagMeta::ALBUM);
}

QString MusicSongMeta::comment() noexcept
{
    return songMeta()->m_metaData[TagMeta::COMMENT];
}

QString MusicSongMeta::year() noexcept
{
    return songMeta()->m_metaData[TagMeta::YEAR];
}

QString MusicSongMeta::trackNum() noexcept
{
    const QString &v = songMeta()->m_metaData[TagMeta::TRACK];
    bool ok = false;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? TTK_DEFAULT_STR : v;
    }
    return TTK_DEFAULT_STR;
}

QString MusicSongMeta::genre() noexcept
{
    return formatString(TagMeta::GENRE);
}

QString MusicSongMeta::rating() noexcept
{
    return formatString(TagMeta::RATING);
}

QString MusicSongMeta::channel() noexcept
{
    return songMeta()->m_metaData[TagMeta::CHANNEL];
}

QString MusicSongMeta::description() noexcept
{
    return songMeta()->m_metaData[TagMeta::DESCRIPTION];
}

void MusicSongMeta::setArtist(const QString &artist) noexcept
{
    songMeta()->m_metaData[TagMeta::ARTIST] = artist;
}

void MusicSongMeta::setTitle(const QString &title) noexcept
{
    songMeta()->m_metaData[TagMeta::TITLE] = title;
}

void MusicSongMeta::setAlbum(const QString &album) noexcept
{
    songMeta()->m_metaData[TagMeta::ALBUM] = album;
}

void MusicSongMeta::setComment(const QString &comment) noexcept
{
    songMeta()->m_metaData[TagMeta::COMMENT] = comment;
}

void MusicSongMeta::setYear(const QString &year) noexcept
{
    songMeta()->m_metaData[TagMeta::YEAR] = year;
}

void MusicSongMeta::setTrackNum(const QString &track) noexcept
{
    songMeta()->m_metaData[TagMeta::TRACK] = track;
}

void MusicSongMeta::setGenre(const QString &genre) noexcept
{
    songMeta()->m_metaData[TagMeta::GENRE] = genre;
}

void MusicSongMeta::setRating(const QString &rating) noexcept
{
    songMeta()->m_metaData[TagMeta::RATING] = rating;
}

void MusicSongMeta::setCover(const QPixmap &cover) noexcept
{
#if TTK_VERSION >= TTK_VERSION_CHECK(2,5,3,0)
    if(cover.width() > 500 || cover.height() > 500)
    {
        songMeta()->m_cover = cover.scaled(500, 500, Qt::KeepAspectRatio);
    }
    else
    {
        songMeta()->m_cover = cover;
    }
#else
    Q_UNUSED(cover);
#endif
}

void MusicSongMeta::setCover(const QByteArray &data) noexcept
{
    if(data.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
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

QPixmap MusicSongMeta::cover() noexcept
{
#if TTK_VERSION >= TTK_VERSION_CHECK(2,5,3,0)
    return songMeta()->m_cover;
#else
    return QPixmap();
#endif
}

QString MusicSongMeta::lyrics() noexcept
{
    return songMeta()->m_lyrics;
}

QString MusicSongMeta::sampleRate() noexcept
{
    return songMeta()->m_metaData[TagMeta::SAMPLERATE];
}

QString MusicSongMeta::bitrate() noexcept
{
    const QString &bitrate = songMeta()->m_metaData[TagMeta::BITRATE];
    return bitrate.isEmpty() ? TTK_DEFAULT_STR : bitrate + " kbps";
}

QString MusicSongMeta::duration() noexcept
{
    return songMeta()->m_metaData[TagMeta::LENGTH];
}

MusicSongMeta::MusicSongMeta(const MusicSongMeta &other) noexcept
{
    if(this == &other)
    {
        return;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;

    for(const MusicMeta *meta : qAsConst(m_songMetas))
    {
        m_songMetas << new MusicMeta(*meta);
    }
}

MusicSongMeta::MusicSongMeta(MusicSongMeta &&other) noexcept
    : m_offset(other.m_offset),
      m_path(other.m_path),
      m_songMetas(std::move(other.m_songMetas))
{

}

MusicSongMeta& MusicSongMeta::operator= (const MusicSongMeta &other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    m_offset = other.m_offset;
    m_path = other.m_path;

    for(const MusicMeta *meta : qAsConst(m_songMetas))
    {
        m_songMetas << new MusicMeta(*meta);
    }

    return *this;
}

MusicSongMeta& MusicSongMeta::operator= (MusicSongMeta &&other) noexcept
{
    if(this == &other)
    {
        return *this;
    }

    std::swap(m_offset, other.m_offset);
    std::swap(m_path, other.m_path);
    std::swap(m_songMetas, other.m_songMetas);
    return *this;
}

void MusicSongMeta::setSongMetaIndex(int index) noexcept
{
    if(index < 0 || index >= songMetaCount())
    {
        return;
    }

    m_offset = index;
}

int MusicSongMeta::songMetaCount() const noexcept
{
    return m_songMetas.count();
}

void MusicSongMeta::clearSongMeta() noexcept
{
    qDeleteAll(m_songMetas);
    m_songMetas.clear();
    m_offset = -1;
}

MusicMeta *MusicSongMeta::songMeta() noexcept
{
    if(m_songMetas.isEmpty())
    {
        m_songMetas << new MusicMeta;
        m_offset = 0;
    }

    if(m_offset < 0 || m_offset >= songMetaCount())
    {
        return nullptr;
    }

    return m_songMetas[m_offset];
}

QString MusicSongMeta::formatString(TagMeta::Type type) noexcept
{
    const QString &v = songMeta()->m_metaData[type];
    return TTK::String::charactersReplace(v);
}


#ifdef Q_OS_UNIX
static constexpr const char *SPLITER = "*******************************************************************\n";
#else
static constexpr const char *SPLITER = "************************************************************************\n";
#endif

bool MusicSongMeta::readInformation()
{
    clearSongMeta();

    DecoderFactory *factory = Decoder::findByFilePath(m_path);
    if(factory)
    {
        qint64 length = 0;
        QStringList files;
        const QList<TrackInfo*> infos(factory->createPlayList(m_path, TrackInfo::AllParts, &files));

        for(TrackInfo *info : qAsConst(infos))
        {
            MusicMeta *meta = new MusicMeta;
            meta->m_path = info->path();
            meta->m_metaData[TagMeta::URL] = files.isEmpty() ? meta->m_path : files.front();

            meta->m_metaData[TagMeta::SAMPLERATE] = info->value(Qmmp::SAMPLERATE);
            meta->m_metaData[TagMeta::BITRATE] = info->value(Qmmp::BITRATE);
            meta->m_metaData[TagMeta::CHANNEL] = info->value(Qmmp::CHANNELS);

            meta->m_metaData[TagMeta::TITLE] = info->value(Qmmp::TITLE);
            meta->m_metaData[TagMeta::ARTIST] = info->value(Qmmp::ARTIST);
            meta->m_metaData[TagMeta::ALBUM] = info->value(Qmmp::ALBUM);
            meta->m_metaData[TagMeta::YEAR] = info->value(Qmmp::YEAR);
            meta->m_metaData[TagMeta::COMMENT] = info->value(Qmmp::COMMENT);
            meta->m_metaData[TagMeta::TRACK] = info->value(Qmmp::TRACK);
            meta->m_metaData[TagMeta::GENRE] = info->value(Qmmp::GENRE);

            length = info->duration();
            if(length == 0)
            {
                const int bitrate = info->value(Qmmp::BITRATE).toInt();
                if(bitrate > 0)
                {
                    length = QFileInfo(meta->m_metaData[TagMeta::URL]).size() * 8.0f / bitrate;
                }
            }

            meta->m_metaData[TagMeta::LENGTH] = TTKTime::formatDuration(length);

            m_songMetas << meta;
            m_offset = m_songMetas.count() - 1;
        }
        qDeleteAll(infos);

        if(m_songMetas.isEmpty())
        {
            return false;
        }

        QString description;
        const DecoderProperties &properties = factory->properties();

        description += "ShortName: " + properties.shortName + TTK_LINEFEED;
        description += "DecoderName: " + properties.name + TTK_LINEFEED;
        description += "Description: " + properties.description + TTK_LINEFEED;
        description += SPLITER;

        const MetaDataModel *model = factory->createMetaDataModel(m_path, true);
        if(model)
        {
            songMeta()->m_cover = model->cover();
            songMeta()->m_lyrics = model->lyrics();

            for(const MetaDataItem &item : model->extraProperties())
            {
                if(item.name() == "Rating")
                {
                    songMeta()->m_metaData[TagMeta::RATING] = item.value().toString();
                }

                QString value = item.value().toString();
                if(value.contains(TTK_LINEFEED))
                {
                    value = TTK_LINEFEED + value;
                }

                description += item.name() + ": " + value + TTK_LINEFEED;
                description += SPLITER;
            }

            for(const MetaDataItem &item : model->descriptions())
            {
                QString value = item.value().toString();
                if(value.contains(TTK_LINEFEED))
                {
                    value = TTK_LINEFEED + value;
                }

                description += item.name() + ": " + value + TTK_LINEFEED;
                description += SPLITER;
            }

            delete model;
        }

        songMeta()->m_metaData[TagMeta::DESCRIPTION] = description;
    }

    return !m_songMetas.isEmpty();
}

bool MusicSongMeta::saveInformation()
{
    DecoderFactory *factory = Decoder::findByFilePath(m_path);
    if(!factory)
    {
        return false;
    }

    MetaDataModel *model = factory->createMetaDataModel(m_path, false);
    if(model)
    {
        const QList<TagModel* > &tags = model->tags();
        if(!tags.isEmpty())
        {
            TagModel *tagModel = tags.front();
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
