#include "decodersndfilefactory.h"
#include "sndfilemetadatamodel.h"
#include "decoder_sndfile.h"

#include <QFileInfo>

#ifndef WAVE_FORMAT_PCM
#  define WAVE_FORMAT_PCM 0x0001
#endif
#define WAVE_FORMAT_ADPCM 0x0002
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_ALAW 0x0006
#define WAVE_FORMAT_MULAW 0x0007
#define WAVE_FORMAT_EXTENSIBLE 0xfffe

static QMap<Qmmp::MetaData, QString> id3v2toMetaData(TagLib::ID3v2::Tag *tag)
{
    QMap<Qmmp::MetaData, QString> metaData;
    metaData.insert(Qmmp::ARTIST, TStringToQString(tag->artist()));
    metaData.insert(Qmmp::ALBUM, TStringToQString(tag->album()));
    metaData.insert(Qmmp::COMMENT, TStringToQString(tag->comment()));
    metaData.insert(Qmmp::GENRE, TStringToQString(tag->genre()));
    metaData.insert(Qmmp::TITLE, TStringToQString(tag->title()));
    metaData.insert(Qmmp::YEAR, QString::number(tag->year()));
    metaData.insert(Qmmp::TRACK, QString::number(tag->track()));

    if(!tag->frameListMap()["TPE2"].isEmpty())
    {
        TagLib::String albumArtist = tag->frameListMap()["TPE2"].front()->toString();
        metaData.insert(Qmmp::ALBUMARTIST, TStringToQString(albumArtist));
    }

    if(!tag->frameListMap()["TCOM"].isEmpty())
    {
        TagLib::String composer = tag->frameListMap()["TCOM"].front()->toString();
        metaData.insert(Qmmp::COMPOSER, TStringToQString(composer));
    }

    if(!tag->frameListMap()["TPOS"].isEmpty())
    {
        TagLib::String disc = tag->frameListMap()["TPOS"].front()->toString();
        metaData.insert(Qmmp::DISCNUMBER, TStringToQString(disc));
    }

    return metaData;
}

bool DecoderSndFileFactory::canDecode(QIODevice *input) const
{
    char buf[36] = {0};
    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if(!memcmp(buf + 8, "WAVE", 4) && (!memcmp(buf, "RIFF", 4) || !memcmp(buf, "RIFX", 4)))
    {
        quint16 subformat = 0;
        if(!memcmp(buf + 12, "fmt ", 4))
        {
            subformat = buf[21] << 8 | buf[20];
        }
        else if(!input->isSequential())
        {
            input->seek(12);
            //skip "JUNK", "bext" and "fact" chunks
            while(!input->atEnd())
            {
                if(input->peek(buf, sizeof(buf)) != sizeof(buf))
                    return false;

                if(!memcmp(buf, "fmt ", 4))
                {
                    subformat = (quint16(buf[9]) << 8) + buf[8];
                    break;
                }
                else if(!memcmp(buf, "JUNK", 4) || !memcmp(buf, "bext", 4) || !memcmp(buf, "fact", 4))
                {
                    size_t size = buf[4] | buf[5] << 8 | buf[6] << 16 | buf[7] << 24;
                    if(!input->seek(input->pos() + size + 8))
                        break;
                }
                else
                {
                    break;
                }
            }
            input->seek(0);
        }

        switch(subformat)
        {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_ADPCM:
        case WAVE_FORMAT_IEEE_FLOAT:
        case WAVE_FORMAT_ALAW:
        case WAVE_FORMAT_MULAW:
        case WAVE_FORMAT_EXTENSIBLE:
            return true;
        default:
            return false;
        }
    }
    else if(!memcmp(buf, "FORM", 4))
    {
        if(!memcmp(buf + 8, "AIFF", 4) || !memcmp(buf + 8, "AIFC", 4))
            return true;
        if(!memcmp(buf + 8, "8SVX", 4))
            return true;
    }
    else if(!memcmp(buf, ".snd", 4) || !memcmp(buf, "dns.", 4))
        return true;
    else if(!memcmp(buf, "fap ", 4) || !memcmp(buf, " paf", 4))
        return true;
    else if(!memcmp(buf, "NIST", 4))
        return true;
    else if(!memcmp(buf, "Crea", 4) && !memcmp(buf + 4, "tive", 4))
        return true;
    else if(!memcmp(buf, "riff", 4))
        return true;
    return false;
}

DecoderProperties DecoderSndFileFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Sndfile Plugin");
    properties.shortName = "sndfile";
    properties.filters << "*.wav" << "*.au" << "*.snd" << "*.aif" << "*.aiff" << "*.8svx";
    properties.filters << "*.sph" << "*.sf" << "*.voc" << "*.w64";
    properties.description = "PCM File";
    return properties;
}

Decoder *DecoderSndFileFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderSndFile(input);
}

QList<TrackInfo*> DecoderSndFileFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::FileStream stream(QStringToFileName(path), true);

    //AIFF and WAVE are supported by TagLib
    if(TagLib::RIFF::AIFF::File::isSupported(&stream))
    {
        TagLib::RIFF::AIFF::File file(&stream);

        if((parts & TrackInfo::MetaData) && file.tag() && !file.tag()->isEmpty())
            info->setValues(id3v2toMetaData(file.tag()));

        if((parts & TrackInfo::Properties) && file.audioProperties())
        {
            info->setValue(Qmmp::BITRATE, file.audioProperties()->bitrate());
            info->setValue(Qmmp::SAMPLERATE, file.audioProperties()->sampleRate());
            info->setValue(Qmmp::CHANNELS, file.audioProperties()->channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, file.audioProperties()->bitsPerSample());
            if(file.audioProperties()->isAiffC() && !file.audioProperties()->compressionName().isEmpty())
                info->setValue(Qmmp::FORMAT_NAME, QString("AIFF-C (%1)").arg(TStringToQString(file.audioProperties()->compressionName())));
            else if(file.audioProperties()->isAiffC())
                info->setValue(Qmmp::FORMAT_NAME, "AIFF-C"); //unknown compression
            else
                info->setValue(Qmmp::FORMAT_NAME, "AIFF");
            info->setDuration(file.audioProperties()->lengthInMilliseconds());
        }

        return QList<TrackInfo*>() << info;
    }

    if(TagLib::RIFF::WAV::File::isSupported(&stream))
    {
        TagLib::RIFF::WAV::File file(&stream);

        if((parts & TrackInfo::MetaData) && file.ID3v2Tag() && !file.ID3v2Tag()->isEmpty())
            info->setValues(id3v2toMetaData(file.ID3v2Tag()));

        if((parts & TrackInfo::Properties) && file.audioProperties())
        {
            info->setValue(Qmmp::BITRATE, file.audioProperties()->bitrate());
            info->setValue(Qmmp::SAMPLERATE, file.audioProperties()->sampleRate());
            info->setValue(Qmmp::CHANNELS, file.audioProperties()->channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, file.audioProperties()->bitsPerSample());
            SF_FORMAT_INFO format_info = { (file.audioProperties()->format()) << 16 & SF_FORMAT_TYPEMASK, nullptr, nullptr };
            sf_command(nullptr, SFC_GET_FORMAT_INFO, &format_info, sizeof(format_info));
            info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(format_info.name));
            info->setDuration(file.audioProperties()->lengthInMilliseconds());
        }

        return QList<TrackInfo*>() << info;
    }

    //fallback
    SF_INFO snd_info;
    memset(&snd_info, 0, sizeof(snd_info));
    snd_info.format = 0;
#ifdef Q_OS_WIN
    SNDFILE *sndfile = sf_wchar_open(reinterpret_cast<LPCWSTR>(path.utf16()), SFM_READ, &snd_info);
#else
    SNDFILE *sndfile = sf_open(qPrintable(path), SFM_READ, &snd_info);
#endif
    if(!sndfile)
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        const char *title = sf_get_string(sndfile, SF_STR_TITLE);
        info->setValue(Qmmp::TITLE, title ? QString::fromUtf8(title) : QString());
        const char *date = sf_get_string(sndfile, SF_STR_DATE);
        info->setValue(Qmmp::YEAR, date ? QString::fromUtf8(date) : QString());
        const char *album = sf_get_string(sndfile, SF_STR_ALBUM);
        info->setValue(Qmmp::ALBUM, album ? QString::fromUtf8(album) : QString());
        const char *track = sf_get_string(sndfile, SF_STR_TRACKNUMBER);
        info->setValue(Qmmp::TRACK, track ? QString::fromUtf8(track) : QString());
        const char *artist = sf_get_string(sndfile, SF_STR_ARTIST);
        info->setValue(Qmmp::ARTIST, artist ? QString::fromUtf8(artist) : QString());
        const char *comment = sf_get_string(sndfile, SF_STR_COMMENT);
        info->setValue(Qmmp::COMMENT, comment ? QString::fromUtf8(comment) : QString());
        const char *genre = sf_get_string(sndfile, SF_STR_GENRE);
        info->setValue(Qmmp::COMMENT, genre ? QString::fromUtf8(genre) : QString());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setDuration(int(snd_info.frames * 1000 / snd_info.samplerate));
        info->setValue(Qmmp::BITRATE, static_cast<int>(QFileInfo(path).size() * 8.0 / info->duration() + 1.0f));
        info->setValue(Qmmp::SAMPLERATE, snd_info.samplerate);
        info->setValue(Qmmp::CHANNELS, snd_info.channels);

        switch(snd_info.format & SF_FORMAT_SUBMASK)
        {
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_U8:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 8);
            break;
        case SF_FORMAT_PCM_16:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
            break;
        case SF_FORMAT_PCM_24:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 24);
            break;
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 32);
            break;
        case SF_FORMAT_DOUBLE:
            info->setValue(Qmmp::BITS_PER_SAMPLE, 64);
            break;
        }

        SF_FORMAT_INFO format_info;
        memset(&format_info, 0, sizeof(format_info));
        format_info.format = (snd_info.format & SF_FORMAT_TYPEMASK);
        sf_command(nullptr, SFC_GET_FORMAT_INFO, &format_info, sizeof(format_info));
        info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(format_info.name));
    }

    sf_close(sndfile);
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderSndFileFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return new SndFileMetaDataModel(path, readOnly);
}

QDialog *DecoderSndFileFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sndfile, DecoderSndFileFactory)
#endif
