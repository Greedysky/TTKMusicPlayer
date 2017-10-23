#include "musicsongtag.h"
#include "musictime.h"
#include "musicformats.h"
#include "musicversion.h"
#include "musiccoreutils.h"
#include "musicwidgetutils.h"

#include <QStringList>
#include <QPluginLoader>
#include <QFileInfo>
///qmmp incldue
#include "tagreadandwrite.h"
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

QString MusicSongTag::getClassName()
{
    return "MusicSongTag";
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
    return m_parameters[TagReadAndWrite::TAG_ARTIST].toString();
}

QString MusicSongTag::getTitle() const
{
    return m_parameters[TagReadAndWrite::TAG_TITLE].toString();
}

QString MusicSongTag::getAlbum() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUM].toString();
}

QString MusicSongTag::getComment() const
{
    return m_parameters[TagReadAndWrite::TAG_COMMENT].toString();
}

QString MusicSongTag::getYear() const
{
    return m_parameters[TagReadAndWrite::TAG_YEAR].toString();
}

QString MusicSongTag::getTrackNum() const
{
    QString v = m_parameters[TagReadAndWrite::TAG_TRACK].toString();
    bool ok = true;
    if(v.toInt(&ok) > 0)
    {
        return !ok ? "-" : v;
    }
    return "-";
}

QString MusicSongTag::getGenre() const
{
    return m_parameters[TagReadAndWrite::TAG_GENRE].toString();
}

QString MusicSongTag::getAlbumArtist() const
{
    return m_parameters[TagReadAndWrite::TAG_ALBUMARTIST].toString();
}

QString MusicSongTag::getComposer() const
{
    return m_parameters[TagReadAndWrite::TAG_COMPOSER].toString();
}

QString MusicSongTag::getChannel() const
{
    return m_parameters[TagReadAndWrite::TAG_CHANNEL].toString();
}

QString MusicSongTag::getURL() const
{
    return m_parameters[TagReadAndWrite::TAG_URL].toString();
}

/////////////////////////////////////////////
void MusicSongTag::setArtist(const QString &artist)
{
    m_parameters[TagReadAndWrite::TAG_ARTIST] = artist;
}

void MusicSongTag::setTitle(const QString &title)
{
    m_parameters[TagReadAndWrite::TAG_TITLE] = title;
}

void MusicSongTag::setAlbum(const QString &album)
{
    m_parameters[TagReadAndWrite::TAG_ALBUM] = album;
}

void MusicSongTag::setComment(const QString &comment)
{
    m_parameters[TagReadAndWrite::TAG_COMMENT] = comment;
}

void MusicSongTag::setYear(const QString &year)
{
    m_parameters[TagReadAndWrite::TAG_YEAR] = year;
}

void MusicSongTag::setTrackNum(const QString &track)
{
    m_parameters[TagReadAndWrite::TAG_TRACK] = track;
}

void MusicSongTag::setGenre(const QString &genre)
{
    m_parameters[TagReadAndWrite::TAG_GENRE] = genre;
}

void MusicSongTag::setCover(const QPixmap &pix)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap p(pix);
    if(p.width() > 500 || p.height() > 500)
    {
        p = p.scaled(500, 500, Qt::KeepAspectRatio);
    }
    m_parameters[TagReadAndWrite::TAG_COVER] = p;
#else
    Q_UNUSED(data);
#endif
}

void MusicSongTag::setCover(const QByteArray &data)
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    QPixmap pix;
    pix.loadFromData(data);
    if(pix.width() > 500 || pix.height() > 500)
    {
        pix = pix.scaled(500, 500, Qt::KeepAspectRatio);
    }
    m_parameters[TagReadAndWrite::TAG_COVER] = pix;
#else
    Q_UNUSED(data);
#endif
}

QPixmap MusicSongTag::getCover() const
{
#if TTKMUSIC_VERSION >= TTKMUSIC_VERSION_CHECK(2,5,3,0)
    return m_parameters[TagReadAndWrite::TAG_COVER].value<QPixmap>();
#else
    return QPixmap();
#endif
}

/////////////////////////////////////////////
QString MusicSongTag::getSamplingRate() const
{
    return m_parameters[TagReadAndWrite::TAG_SAMPLERATE].toString();
}

QString MusicSongTag::getFormat() const
{
    return m_parameters[TagReadAndWrite::TAG_FORMAT].toString();
}

QString MusicSongTag::getMode() const
{
    return m_parameters[TagReadAndWrite::TAG_MODE].toString();
}

QString MusicSongTag::getBitrate() const
{
    return m_parameters[TagReadAndWrite::TAG_BITRATE].toString();
}

QString MusicSongTag::getLengthString() const
{
    return MusicTime::msecTime2LabelJustified(
           m_parameters[TagReadAndWrite::TAG_LENGTH].toULongLong());
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
    if(obj && (decoderfac = MObject_cast(DecoderFactory*, obj)))
    {
        int length = 0;
        MetaDataModel *model = decoderfac->createMetaDataModel(m_filePath);
        if(model != nullptr)
        {
            QHash<QString, QString> datas = model->audioProperties();
            MusicTime t = MusicTime::fromString(datas.value("Length"), QString("m:ss"));
            length = t.getTimeStamp(MusicTime::All_Msec);
            if(length != 0)
            {
                m_parameters.insert(TagReadAndWrite::TAG_LENGTH, QString::number(length));
            }
            m_parameters.insert(TagReadAndWrite::TAG_SAMPLERATE, datas.value("Sample rate"));
            m_parameters.insert(TagReadAndWrite::TAG_BITRATE, datas.value("Bitrate"));
            m_parameters.insert(TagReadAndWrite::TAG_CHANNEL, datas.value("Channels"));

            m_parameters.insert(TagReadAndWrite::TAG_COVER, model->cover());

            QList<TagModel* > tags = model->tags();
            if(!tags.isEmpty())
            {
                TagModel *tagModel = tags.first();
                if(tags.count() == 3)
                {
                    tagModel = tags[1]; //id3v2 mode tag
                }

                m_parameters[TagReadAndWrite::TAG_TITLE] = tagModel->value(Qmmp::TITLE);
                m_parameters[TagReadAndWrite::TAG_ARTIST] = tagModel->value(Qmmp::ARTIST);
                m_parameters[TagReadAndWrite::TAG_ALBUM] = tagModel->value(Qmmp::ALBUM);
                m_parameters[TagReadAndWrite::TAG_YEAR] = tagModel->value(Qmmp::YEAR);
                m_parameters[TagReadAndWrite::TAG_COMMENT] = tagModel->value(Qmmp::COMMENT);
                m_parameters[TagReadAndWrite::TAG_TRACK] = tagModel->value(Qmmp::TRACK);
                m_parameters[TagReadAndWrite::TAG_GENRE] = tagModel->value(Qmmp::GENRE);
            }
        }

        if(length == 0)
        {
            QList<FileInfo*> infos(decoderfac->createPlayList(m_filePath, true, 0));
            if(!infos.isEmpty())
            {
                length = infos.first()->length()*MT_S2MS;
            }
            qDeleteAll(infos);

            if(length == 0)
            {
                TagReadAndWrite tag;
                if(tag.readFile(m_filePath))
                {
                    QMap<TagReadAndWrite::MusicTag, QString> data = tag.getMusicTags();
                    length = data[TagReadAndWrite::TAG_LENGTH].toInt();
                }
            }

            m_parameters[TagReadAndWrite::TAG_LENGTH] = QString::number(length);
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
    if(obj && (decoderfac = MObject_cast(DecoderFactory*, obj)))
    {
        status = true;
        MetaDataModel *model = decoderfac->createMetaDataModel(m_filePath);
        if(model)
        {
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
            QPixmap pix = getCover();
            if(!pix.isNull())
            {
                model->setCover(MusicUtils::Widget::getPixmapData(pix));
            }
            ////////////////////////////////////////////////////////////////////
        }
        delete model;
        loader.unload();
    }

    return status;
}
