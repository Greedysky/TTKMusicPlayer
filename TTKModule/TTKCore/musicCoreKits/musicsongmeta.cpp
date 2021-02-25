#include "musicsongmeta.h"
#include "musictime.h"
#include "musicformats.h"
#include "ttkversion.h"
#include "musicqmmputils.h"
#include "musicwidgetutils.h"
#include "musicstringutils.h"

///qmmp incldue
#include "decoderfactory.h"
#include "metadatamodel.h"
#include "decoder.h"


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
    if(SongTrackValid(file))
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

    m_filePath = path;
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

QString MusicSongMeta::getDecoder() const
{
    const QString &suffix = QFileInfo(m_filePath).suffix().toLower();
    const TTKStringListMap formats(MusicFormats::supportFormatsStringMap());
    for(const QString &key : formats.keys())
    {
        if(formats.value(key).contains(suffix))
        {
            return QFileInfo(MusicUtils::QMMP::pluginPath("Input", key)).baseName();
        }
    }

    return QString();
}

QString MusicSongMeta::getFilePath() const
{
    return m_filePath;
}

QString MusicSongMeta::getFileBasePath()
{
    return getSongMeta()->m_fileUrl;
}

QString MusicSongMeta::getFileRelatedPath()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_URL];
}

QString MusicSongMeta::getArtist()
{
    return findLegalDataString(TagWrapper::TAG_ARTIST);
}

QString MusicSongMeta::getTitle()
{
    return findLegalDataString(TagWrapper::TAG_TITLE);
}

QString MusicSongMeta::getAlbum()
{
    return findLegalDataString(TagWrapper::TAG_ALBUM);
}

QString MusicSongMeta::getComment()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_COMMENT];
}

QString MusicSongMeta::getYear()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_YEAR];
}

QString MusicSongMeta::getTrackNum()
{
    const QString &v = getSongMeta()->m_metaData[TagWrapper::TAG_TRACK];
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? STRING_NULL : v;
    }
    return STRING_NULL;
}

QString MusicSongMeta::getGenre()
{
    return findLegalDataString(TagWrapper::TAG_GENRE);
}

QString MusicSongMeta::getChannel()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_CHANNEL];
}

void MusicSongMeta::setArtist(const QString &artist)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_ARTIST] = artist;
}

void MusicSongMeta::setTitle(const QString &title)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_TITLE] = title;
}

void MusicSongMeta::setAlbum(const QString &album)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_ALBUM] = album;
}

void MusicSongMeta::setComment(const QString &comment)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_COMMENT] = comment;
}

void MusicSongMeta::setYear(const QString &year)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_YEAR] = year;
}

void MusicSongMeta::setTrackNum(const QString &track)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_TRACK] = track;
}

void MusicSongMeta::setGenre(const QString &genre)
{
    getSongMeta()->m_metaData[TagWrapper::TAG_GENRE] = genre;
}

void MusicSongMeta::setCover(const QPixmap &pix)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap p(pix);
    if(p.width() > 500 || p.height() > 500)
    {
        p = p.scaled(500, 500, Qt::KeepAspectRatio);
    }
    getSongMeta()->m_cover = p;
#else
    Q_UNUSED(data);
#endif
}

void MusicSongMeta::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    setCover(pix);
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongMeta::getCover()
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    return getSongMeta()->m_cover;
#else
    return QPixmap();
#endif
}

QString MusicSongMeta::getSampleRate()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_SAMPLERATE];
}

QString MusicSongMeta::getBitrate()
{
    const QString &bitrate = getSongMeta()->m_metaData[TagWrapper::TAG_BITRATE];
    return bitrate.isEmpty() ? STRING_NULL : bitrate + " kbps";
}

QString MusicSongMeta::getLengthString()
{
    return getSongMeta()->m_metaData[TagWrapper::TAG_LENGTH];
}

MusicSongMeta::MusicSongMeta(const MusicSongMeta &other)
{
    if(this == &other)
    {
        return;
    }

    m_offset = other.m_offset;
    m_filePath = other.m_filePath;
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
    m_filePath = other.m_filePath;
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
    m_filePath = other.m_filePath;
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
    m_filePath = other.m_filePath;
    m_songMetas = other.m_songMetas;
    other.m_songMetas.clear();

    return *this;
}

bool MusicSongMeta::SongTrackValid(const QString &file)
{
    QStringList list;
    list << MUSIC_CUE_FILE "://";
    list << MUSIC_APE_FILE "://";
    list << MUSIC_FFMPEG_FILE "://";
    list << MUSIC_M4B_FILE "://";
    list << MUSIC_FLAC_FILE "://";
    list << MUSIC_GME_FILE "://";
    list << MUSIC_SID_FILE "://";
    list << MUSIC_WVPACK_FILE "://";

    for(const QString &path : qAsConst(list))
    {
        if(file.startsWith(path))
        {
            return true;
        }
    }
    return false;
}

bool MusicSongMeta::SongTrackTpyeContains(const QString &file)
{
    QStringList list;
    list << MUSIC_CUE_FILE;
    list << MUSIC_APE_FILE;
    list << MUSIC_FFMPEG_FILE;
    list << MUSIC_M4B_FILE;
    list << MUSIC_FLAC_FILE;
    list << MUSIC_GME_FILE;
    list << MUSIC_SID_FILE;
    list << MUSIC_WVPACK_FILE;

    return list.contains(file);
}

void MusicSongMeta::setSongMetaIndex(int index)
{
    if(index < 0 || index >= m_songMetas.size())
    {
        return;
    }

    m_offset = index;
}

int MusicSongMeta::getSongMetaSize() const
{
    return m_songMetas.size();
}

void MusicSongMeta::clearSongMeta()
{
    qDeleteAll(m_songMetas);
    m_offset = -1;
}

MusicMeta *MusicSongMeta::getSongMeta()
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
    const QString &v = getSongMeta()->m_metaData[type];
    return MusicUtils::String::illegalCharactersReplaced(v);
}

bool MusicSongMeta::readInformation()
{
    clearSongMeta();
    DecoderFactory *factory = Decoder::findByFilePath(m_filePath);

    if(factory)
    {
        QPixmap cover;
        MetaDataModel *model = factory->createMetaDataModel(m_filePath, true);
        if(model)
        {
            cover = model->cover();
            delete model;
        }

        qint64 length = 0;
        QStringList files;
        const QList<TrackInfo*> infos(factory->createPlayList(m_filePath, TrackInfo::AllParts, &files));

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
            getSongMeta()->m_cover = cover;

            if(length == 0)
            {
                TagWrapper wrapper;
                if(wrapper.readFile(m_filePath))
                {
                    const QMap<TagWrapper::Type, QString> &data = wrapper.getMusicTags();
                    length = data[TagWrapper::TAG_LENGTH].toLongLong();
                }
                getSongMeta()->m_metaData[TagWrapper::TAG_LENGTH] = MusicTime::msecTime2LabelJustified(length);
            }
        }
    }

    return !m_songMetas.isEmpty();
}

bool MusicSongMeta::saveInformation()
{
    DecoderFactory *factory = Decoder::findByFilePath(m_filePath);

    if(factory)
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
        return true;
    }

    return false;
}
