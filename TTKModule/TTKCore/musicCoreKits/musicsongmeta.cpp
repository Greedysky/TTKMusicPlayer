#include "musicsongmeta.h"
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
#include "decoderfactory.h"
#include "metadatamodel.h"
#include "decoder.h"

MusicSongMeta::MusicSongMeta()
{

}

bool MusicSongMeta::read(const QString &file)
{
    const QFile f(file);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    m_filePath = file;
    return readInformation();
}

bool MusicSongMeta::save()
{
    return saveInformation();
}

QString MusicSongMeta::getDecoder() const
{
    const QString &v = findPluginPath();
    return QFileInfo(v).baseName();
}

QString MusicSongMeta::getFilePath() const
{
    return m_filePath;
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
    return getSongMeta()->m_metaData[TagWrapper::TAG_BITRATE] + " kbps";
}

QString MusicSongMeta::getLengthString()
{
    return MusicTime::msecTime2LabelJustified(getSongMeta()->m_metaData[TagWrapper::TAG_LENGTH].toULongLong());
}

MusicSongMeta::MusicMeta *MusicSongMeta::getSongMeta()
{
    if(m_songMetas.isEmpty())
    {
        m_songMetas << MusicMeta();
    }
    return &m_songMetas[0];
}

QString MusicSongMeta::findLegalDataString(TagWrapper::Type type)
{
    const QString &v = getSongMeta()->m_metaData[type];
    return MusicUtils::String::illegalCharactersReplaced(v);
}

QString MusicSongMeta::findPluginPath() const
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

bool MusicSongMeta::readInformation()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    const QObject *obj = loader.instance();
    DecoderFactory *factory = nullptr;
    m_songMetas.clear();

    if(obj && (factory = TTKObject_cast(DecoderFactory*, obj)))
    {
        QPixmap cover;
        MetaDataModel *model = factory->createMetaDataModel(m_filePath, true);
        if(model)
        {
            cover = model->cover();
            delete model;
        }

        qint64 length = 0;
        const QList<TrackInfo*> infos(factory->createPlayList(m_filePath, TrackInfo::AllParts, nullptr));
        for(TrackInfo *info : qAsConst(infos))
        {
            MusicMeta meta;
            meta.m_filePath = info->path();

            meta.m_metaData[TagWrapper::TAG_SAMPLERATE] = info->value(Qmmp::SAMPLERATE);
            meta.m_metaData[TagWrapper::TAG_BITRATE] = info->value(Qmmp::BITRATE);
            meta.m_metaData[TagWrapper::TAG_CHANNEL] = info->value(Qmmp::CHANNELS);

            meta.m_metaData[TagWrapper::TAG_TITLE] = info->value(Qmmp::TITLE);
            meta.m_metaData[TagWrapper::TAG_ARTIST] = info->value(Qmmp::ARTIST);
            meta.m_metaData[TagWrapper::TAG_ALBUM] = info->value(Qmmp::ALBUM);
            meta.m_metaData[TagWrapper::TAG_YEAR] = info->value(Qmmp::YEAR);
            meta.m_metaData[TagWrapper::TAG_COMMENT] = info->value(Qmmp::COMMENT);
            meta.m_metaData[TagWrapper::TAG_TRACK] = info->value(Qmmp::TRACK);
            meta.m_metaData[TagWrapper::TAG_GENRE] = info->value(Qmmp::GENRE);

            length = info->duration();
            if(length != 0)
            {
                meta.m_metaData[TagWrapper::TAG_LENGTH] = QString::number(length);
            }
            m_songMetas << meta;
        }
        qDeleteAll(infos);

        if(length == 0 && !m_songMetas.isEmpty())
        {
            TagWrapper tag;
            if(tag.readFile(m_filePath))
            {
                const QMap<TagWrapper::Type, QString> &data = tag.getMusicTags();
                length = data[TagWrapper::TAG_LENGTH].toLongLong();
            }
            getSongMeta()->m_metaData[TagWrapper::TAG_LENGTH] = QString::number(length);
        }

        loader.unload();
    }

    return !m_songMetas.isEmpty();
}

bool MusicSongMeta::saveInformation()
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
