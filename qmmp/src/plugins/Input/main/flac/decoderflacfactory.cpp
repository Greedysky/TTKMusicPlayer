#include "decoderflacfactory.h"
#include "decoder_flac.h"
#include "flacmetadatamodel.h"

#include <taglib/id3v2framefactory.h>
#include <qmmp/cueparser.h>

bool DecoderFLACFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if(!memcmp(buf, "ID3", 3))
    {
        TagLib::ByteVector byteVector(buf, sizeof(buf));
        TagLib::ID3v2::Header header(byteVector);

        if(input->isSequential())
        {
            const int peekSize = header.completeTagSize() + sizeof(buf);
            char peekBuf[peekSize];
            if(input->peek(peekBuf, peekSize) != peekSize)
                return false;

            memcpy(buf, peekBuf + header.completeTagSize(), sizeof(buf));
        }
        else
        {
            input->seek(header.completeTagSize());
            const int peekSize = input->peek(buf, sizeof(buf));
            input->seek(0); //restore position

            if(peekSize != sizeof(buf))
                return false;
        }
    }

    if(!memcmp(buf, "fLaC", 4)) //native flac
        return true;

    if(!memcmp(buf, "OggS", 4) && !memcmp(buf + 29, "FLAC", 4)) //ogg flac
        return true;
    return false;
}

DecoderProperties DecoderFLACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("FLAC Plugin");
    properties.shortName = "flac";
    properties.filters << "*.flac" << "*.oga";
    properties.description = "Free Lossless Audio Codec File";
    properties.contentTypes << "audio/x-flac" << "audio/flac";
    properties.protocols << "flac";
    return properties;
}

Decoder *DecoderFLACFactory::create(const QString &path, QIODevice *input)
{
    return new DecoderFLAC(path, input);
}

QList<TrackInfo*> DecoderFLACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    int track = -1; //cue track
    QString filePath = path;

    if(path.contains("://")) //is it cue track?
    {
        filePath = TrackInfo::pathFromUrl(path, &track);
        parts = TrackInfo::AllParts; //extract all metadata for single cue track
    }

    TrackInfo *info = new TrackInfo(filePath);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::Ogg::XiphComment *tag = nullptr;
    TagLib::FLAC::Properties *ap = nullptr;

    TagLib::FLAC::File *flacFile = nullptr;
    TagLib::Ogg::FLAC::File *oggFlacFile = nullptr;

    TagLib::FileStream stream(QStringToFileName(filePath), true);

    if(filePath.endsWith(".flac", Qt::CaseInsensitive))
    {
#if TAGLIB_MAJOR_VERSION >= 2
        flacFile = new TagLib::FLAC::File(&stream);
#else
        flacFile = new TagLib::FLAC::File(&stream, TagLib::ID3v2::FrameFactory::instance());
#endif
        tag = flacFile->xiphComment();
        ap = flacFile->audioProperties();
    }
    else if(filePath.endsWith(".oga", Qt::CaseInsensitive))
    {
        oggFlacFile = new TagLib::Ogg::FLAC::File(&stream);
        tag = oggFlacFile->tag();
        ap = oggFlacFile->audioProperties();
    }
    else
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, ap->bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, flacFile ? "FLAC" : "Ogg FLAC");
        info->setValue(Qmmp::FILE_SIZE, QFileInfo(filePath).size()); //adds file size for cue tracks
        info->setDuration(ap->lengthInMilliseconds());
    }

    if((parts & TrackInfo::ReplayGainInfo) && tag && !tag->isEmpty())
    {
        const TagLib::Ogg::FieldListMap &items = tag->fieldListMap();
        if(items.contains("REPLAYGAIN_TRACK_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN,TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].front()));
        if(items.contains("REPLAYGAIN_TRACK_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK,TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN,TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK,TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].front()));
    }

    if((parts & TrackInfo::MetaData) && tag && !tag->isEmpty())
    {
        if(tag->fieldListMap().contains("CUESHEET") && ap)
        {
            const QByteArray data(tag->fieldListMap()["CUESHEET"].toString().toCString(true));
            QString diskNumber;

            if(tag->contains("DISCNUMBER") && !tag->fieldListMap()["DISCNUMBER"].isEmpty())
            {
                TagLib::StringList fld = tag->fieldListMap()["DISCNUMBER"];
                diskNumber = TStringToQString(fld.toString()).trimmed();
            }

            CueParser parser(data);

            if(!diskNumber.isEmpty())
            {
                for(int i = 1; i <= parser.count(); ++i)
                    parser.setMetaData(i, Qmmp::DISCNUMBER, diskNumber);
            }

            parser.setDuration(ap->lengthInMilliseconds());
            parser.setProperties(info->properties());
            parser.setUrl("flac", filePath);

            delete flacFile;
            delete oggFlacFile;

            delete info;
            return parser.createPlayList(track);
        }
        else if(track > 0) //cue track is not available
        {
            delete flacFile;
            delete oggFlacFile;

            delete info;
            return QList<TrackInfo*>();
        }

        info->setValue(Qmmp::ALBUM, TStringToQString(tag->album()));
        info->setValue(Qmmp::ARTIST, TStringToQString(tag->artist()));
        info->setValue(Qmmp::COMMENT, TStringToQString(tag->comment()));
        info->setValue(Qmmp::GENRE, TStringToQString(tag->genre()));
        info->setValue(Qmmp::TITLE, TStringToQString(tag->title()));
        info->setValue(Qmmp::YEAR, tag->year());
        info->setValue(Qmmp::TRACK, tag->track());
        //additional metadata
        TagLib::StringList fld;
        if(!(fld = tag->fieldListMap()["ALBUMARTIST"]).isEmpty())
            info->setValue(Qmmp::ALBUMARTIST, TStringToQString(fld.toString()));
        if(!(fld = tag->fieldListMap()["COMPOSER"]).isEmpty())
            info->setValue(Qmmp::COMPOSER, TStringToQString(fld.toString()));
        if(!(fld = tag->fieldListMap()["DISCNUMBER"]).isEmpty())
            info->setValue(Qmmp::DISCNUMBER, TStringToQString(fld.toString()));

    }

    delete flacFile;
    delete oggFlacFile;
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderFLACFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    if(!path.contains("://") || path.startsWith("flac://"))
        return new FLACMetaDataModel(path, readOnly);
    else
        return nullptr;
}

QDialog *DecoderFLACFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flac, DecoderFLACFactory)
#endif
