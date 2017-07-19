#include "musicsongtag.h"
#include "musictime.h"
#include "musicformats.h"
#include "musicversion.h"
#include "musiccoreutils.h"

#include <QStringList>
#include <QPluginLoader>
#include <QFileInfo>
///qmmp incldue
#include "decoderfactory.h"
#include "metadatamodel.h"
#include "decoder.h"

MusicSongTag::MusicSongTag()
{
    m_tag = nullptr;
    m_id3v2Version = 3;
}

MusicSongTag::MusicSongTag(const QString &file)
    : MusicSongTag()
{
    readFile(file);
}

MusicSongTag::~MusicSongTag()
{
    delete m_tag;
}

QString MusicSongTag::getClassName()
{
    return "MusicSongTag";
}

bool MusicSongTag::readFile(const QString &file)
{
    QFile f(file);
    if(!f.exists() || f.size() <= 0)
    {
        return false;
    }

    delete m_tag;
    m_tag = new TagReadAndWrite(file);
    m_filePath = file;
    if(!m_tag->readFile())
    {
        return readOtherTaglibNotSupport(file);
    }

    m_parameters = m_tag->getMusicTags();
    return true;
}

QString MusicSongTag::getFilePath() const
{
    return m_filePath;
}

void MusicSongTag::setTagVersion(int id3v2Version)
{
    m_id3v2Version = id3v2Version;
    if(m_id3v2Version != 3 && m_id3v2Version != 4)
    {
        m_id3v2Version = 3;
    }
}

bool MusicSongTag::readOtherTaglibNotSupport(const QString &path)
{
    QPluginLoader loader;
    QString suffix = QFileInfo(path).suffix().toLower();

    MusicObject::MStringsListMap formats(MusicFormats::supportFormatsStringMap());
    foreach(const QString &key, formats.keys())
    {
        if(formats.value(key).contains(suffix))
        {

            QString path = MusicUtils::Core::pluginPath("Input", key);
            loader.setFileName(path);
            break;
        }
    }

    QObject *obj = loader.instance();
    DecoderFactory *decoderfac = nullptr;
    if(obj && (decoderfac = MObject_cast(DecoderFactory*, obj)) )
    {
        MetaDataModel *model = decoderfac->createMetaDataModel(path);
        if(model != nullptr)
        {
            QHash<QString, QString> datas = model->audioProperties();
            MusicTime t = MusicTime::fromString(datas.value("Length"), QString("m:ss"));
            QString ts = QString::number(t.getTimeStamp(MusicTime::All_Msec));
            m_parameters.insert(TagReadAndWrite::TAG_LENGTH, ts);
            m_parameters.insert(TagReadAndWrite::TAG_SAMPLERATE, datas.value("Sample rate"));
            m_parameters.insert(TagReadAndWrite::TAG_BITRATE, datas.value("Bitrate"));
        }

        QList<FileInfo*> infos(decoderfac->createPlayList(path, true, 0));
        if(!infos.isEmpty())
        {
            QString t = QString::number(infos.first()->length()*MT_S2MS);
            m_parameters.insert(TagReadAndWrite::TAG_LENGTH, t);
        }
        loader.unload();
    }

    return !m_parameters.isEmpty();
}

QString MusicSongTag::getArtist() const
{
    return m_parameters[TagReadAndWrite::TAG_ARTIST];
}

QString MusicSongTag::getTitle() const
{
    return m_parameters[TagReadAndWrite::TAG_TITLE];
}

QString MusicSongTag::getAlbum() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUM];
}

QString MusicSongTag::getComment() const
{
    return m_parameters[TagReadAndWrite::TAG_COMMENT];
}

QString MusicSongTag::getYear() const
{
    return m_parameters[TagReadAndWrite::TAG_YEAR];
}

QString MusicSongTag::getTrackNum() const
{
    return m_parameters[TagReadAndWrite::TAG_TRACK];
}

QString MusicSongTag::getGenre() const
{
    return m_parameters[TagReadAndWrite::TAG_GENRE];
}

QString MusicSongTag::getAlbumArtist() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUMARTIST];
}

QString MusicSongTag::getComposer() const
{
    return m_parameters[TagReadAndWrite::TAG_COMPOSER];
}

QString MusicSongTag::getChannel() const
{
    return m_parameters[TagReadAndWrite::TAG_CHANNEL];
}

QString MusicSongTag::getURL() const
{
    return m_parameters[TagReadAndWrite::TAG_URL];
}

/////////////////////////////////////////////
bool MusicSongTag::setArtist(const QString &artist)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_ARTIST, artist, m_id3v2Version);
}

bool MusicSongTag::setTitle(const QString &title)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_TITLE, title, m_id3v2Version);
}

bool MusicSongTag::setAlbum(const QString &album)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_ALBUM, album, m_id3v2Version);
}

bool MusicSongTag::setComment(const QString &comment)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_COMMENT, comment, m_id3v2Version);
}

bool MusicSongTag::setYear(const QString &year)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_YEAR, year, m_id3v2Version);
}

bool MusicSongTag::setTrackNum(const QString &track)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_TRACK, track, m_id3v2Version);
}

bool MusicSongTag::setGenre(const QString &genre)
{
    return m_tag->writeMusicTag(TagReadAndWrite::TAG_GENRE, genre, m_id3v2Version);
}

bool MusicSongTag::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,4,7,0)
    return m_tag->writeCover(data, m_id3v2Version);
#else
    Q_UNUSED(data);
    return false;
#endif
}

QByteArray MusicSongTag::getCover() const
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,1,0)
    return m_tag->getCover();
#else
    return QByteArray();
#endif
}

/////////////////////////////////////////////
QString MusicSongTag::getSamplingRate() const
{
    return m_parameters[TagReadAndWrite::TAG_SAMPLERATE];
}

QString MusicSongTag::getFormat() const
{
    return m_parameters[TagReadAndWrite::TAG_FORMAT];
}

QString MusicSongTag::getMode() const
{
    return m_parameters[TagReadAndWrite::TAG_MODE];
}

QString MusicSongTag::getBitrate() const
{
    return m_parameters[TagReadAndWrite::TAG_BITRATE];
}

QString MusicSongTag::getLengthString() const
{
    return MusicTime::msecTime2LabelJustified(
           m_parameters[TagReadAndWrite::TAG_LENGTH].toULongLong());
}
