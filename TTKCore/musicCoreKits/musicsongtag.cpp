#include "musicsongtag.h"
#include "musictime.h"
#include "musicformats.h"
#include "musicversion.h"
#include "musiccoreutils.h"

#include <QBuffer>
#include <QStringList>
#include <QPluginLoader>
#include <QFileInfo>
///qmmp incldue
#include "decoderfactory.h"
#include "metadatamodel.h"
#include "decoder.h"

MusicSongTag::MusicSongTag()
{

}

MusicSongTag::MusicSongTag(const QString &file)
    : MusicSongTag()
{
    read(file);
}


QString MusicSongTag::getClassName()
{
    return "MusicSongTag";
}

bool MusicSongTag::read(const QString &file)
{
    QFile f(file);
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
    QString v = findPluginPath();
    return QFileInfo(v).baseName();
}

QString MusicSongTag::getFilePath() const
{
    return m_filePath;
}

QString MusicSongTag::getArtist() const
{
    return m_parameters[TAG_ARTIST].toString();
}

QString MusicSongTag::getTitle() const
{
    return m_parameters[TAG_TITLE].toString();
}

QString MusicSongTag::getAlbum() const
{
    return m_parameters[TAG_ALBUM].toString();
}

QString MusicSongTag::getComment() const
{
    return m_parameters[TAG_COMMENT].toString();
}

QString MusicSongTag::getYear() const
{
    return m_parameters[TAG_YEAR].toString();
}

QString MusicSongTag::getTrackNum() const
{
    QString v = m_parameters[TAG_TRACK].toString();
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? "-" : v;
    }
    return "-";
}

QString MusicSongTag::getGenre() const
{
    return m_parameters[TAG_GENRE].toString();
}

QString MusicSongTag::getAlbumArtist() const
{
    return m_parameters[TAG_ALBUMARTIST].toString();
}

QString MusicSongTag::getComposer() const
{
    return m_parameters[TAG_COMPOSER].toString();
}

QString MusicSongTag::getChannel() const
{
    return m_parameters[TAG_CHANNEL].toString();
}

QString MusicSongTag::getURL() const
{
    return m_parameters[TAG_URL].toString();
}

/////////////////////////////////////////////
void MusicSongTag::setArtist(const QString &artist)
{
    m_parameters[TAG_ARTIST] = artist;
}

void MusicSongTag::setTitle(const QString &title)
{
    m_parameters[TAG_TITLE] = title;
}

void MusicSongTag::setAlbum(const QString &album)
{
    m_parameters[TAG_ALBUM] = album;
}

void MusicSongTag::setComment(const QString &comment)
{
    m_parameters[TAG_COMMENT] = comment;
}

void MusicSongTag::setYear(const QString &year)
{
    m_parameters[TAG_YEAR] = year;
}

void MusicSongTag::setTrackNum(const QString &track)
{
    m_parameters[TAG_TRACK] = track;
}

void MusicSongTag::setGenre(const QString &genre)
{
    m_parameters[TAG_GENRE] = genre;
}

void MusicSongTag::setCover(const QPixmap &pix)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,4,7,0)
    m_parameters[TAG_COVER] = pix;
#else
    Q_UNUSED(data);
#endif
}

void MusicSongTag::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    m_parameters[TAG_COVER] = pix;
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongTag::getCover() const
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    return m_parameters[TAG_COVER].value<QPixmap>();
#else
    return QPixmap();
#endif
}

/////////////////////////////////////////////
QString MusicSongTag::getSamplingRate() const
{
    return m_parameters[TAG_SAMPLERATE].toString();
}

QString MusicSongTag::getFormat() const
{
    return m_parameters[TAG_FORMAT].toString();
}

QString MusicSongTag::getMode() const
{
    return m_parameters[TAG_MODE].toString();
}

QString MusicSongTag::getBitrate() const
{
    return m_parameters[TAG_BITRATE].toString();
}

QString MusicSongTag::getLengthString() const
{
    return MusicTime::msecTime2LabelJustified(
           m_parameters[TAG_LENGTH].toULongLong());
}

QString MusicSongTag::findPluginPath() const
{
    QString suffix = QFileInfo(m_filePath).suffix().toLower();

    MusicObject::MStringsListMap formats(MusicFormats::supportFormatsStringMap());
    foreach(const QString &key, formats.keys())
    {
        if(formats.value(key).contains(suffix))
        {
            return MusicUtils::Core::pluginPath("Input", key);
        }
    }

    return QString();
}

bool MusicSongTag::readOtherTaglib()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    QObject *obj = loader.instance();
    DecoderFactory *decoderfac = nullptr;
    if(obj && (decoderfac = MObject_cast(DecoderFactory*, obj)) )
    {
        MetaDataModel *model = decoderfac->createMetaDataModel(m_filePath);
        if(model != nullptr)
        {
            QHash<QString, QString> datas = model->audioProperties();
            MusicTime t = MusicTime::fromString(datas.value("Length"), QString("m:ss"));
            QString ts = QString::number(t.getTimeStamp(MusicTime::All_Msec));
            m_parameters.insert(TAG_LENGTH, ts);
            m_parameters.insert(TAG_SAMPLERATE, datas.value("Sample rate"));
            m_parameters.insert(TAG_BITRATE, datas.value("Bitrate"));
            m_parameters.insert(TAG_CHANNEL, datas.value("Channels"));
        }

        m_parameters.insert(TAG_COVER, model->cover());

        QList<TagModel* > tags = model->tags();
        if(!tags.isEmpty())
        {
            TagModel *tagModel = tags.first();
            if(tags.count() == 3)
            {
                tagModel = tags[1]; //id3v2 mode tag
            }

            m_parameters[TAG_TITLE] = tagModel->value(Qmmp::TITLE);
            m_parameters[TAG_ARTIST] = tagModel->value(Qmmp::ARTIST);
            m_parameters[TAG_ALBUM] = tagModel->value(Qmmp::ALBUM);
            m_parameters[TAG_YEAR] = tagModel->value(Qmmp::YEAR);
            m_parameters[TAG_COMMENT] = tagModel->value(Qmmp::COMMENT);
            m_parameters[TAG_TRACK] = tagModel->value(Qmmp::TRACK);
            m_parameters[TAG_GENRE] = tagModel->value(Qmmp::GENRE);
        }

        delete model;
        loader.unload();
    }

    return !m_parameters.isEmpty();
}

bool MusicSongTag::saveOtherTaglib()
{
    QPluginLoader loader;
    loader.setFileName(findPluginPath());

    bool status = false;
    QObject *obj = loader.instance();
    DecoderFactory *decoderfac = nullptr;
    if(obj && (decoderfac = MObject_cast(DecoderFactory*, obj)) )
    {
        status = true;
        MetaDataModel *model = decoderfac->createMetaDataModel(m_filePath);

        QList<TagModel* > tags = model->tags();
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
            tagModel->save();
        }

        ////////////////////////////////////////////////////////////////////
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        getCover().save(&buffer, JPG_FILE_PREFIX);
        model->setCover(data);
        ////////////////////////////////////////////////////////////////////

        delete model;
        loader.unload();
    }

    return status;
}
