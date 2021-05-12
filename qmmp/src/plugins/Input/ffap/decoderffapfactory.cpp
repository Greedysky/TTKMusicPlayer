#include <taglib/apefile.h>
#include <taglib/apetag.h>
#include <taglib/tfilestream.h>
#include <qmmp/cueparser.h>
#include "ffapmetadatamodel.h"
#include "decoderffapfactory.h"
#include "decoder_ffap.h"
#include "decoder_ffapcue.h"

bool DecoderFFapFactory::canDecode(QIODevice *input) const
{
    char buf[3];
    return (input->peek(buf, 3) == 3 && !memcmp(buf, "MAC", 3));
}

DecoderProperties DecoderFFapFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "FFap Plugin";
    properties.shortName = "ffap";
    properties.filters << "*.ape";
    properties.description = "Monkey's Audio File";
    properties.protocols << "ape";
    properties.priority = 9;
    return properties;
}

Decoder *DecoderFFapFactory::create(const QString &path, QIODevice *input)
{
    if(path.startsWith("ape://"))
        return new DecoderFFapCUE(path);
    else
        return new DecoderFFap(path, input);
}

QList<TrackInfo*> DecoderFFapFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredFiles)
{
    int track = -1; //cue track
    QString filePath = path;

    if(path.contains("://"))    //extract metadata of one cue track
    {
        filePath.remove("ape://");
        filePath.remove(RegularWrapper("#\\d+$"));
        track = path.section("#", -1).toInt();
        parts = TrackInfo::AllParts; //extract all metadata for single cue track
    }
    else
    {
        if(ignoredFiles)
            ignoredFiles->push_back(path);
    }

    TrackInfo *info = new TrackInfo(filePath);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::FileStream stream(QStringToFileName(filePath), true);
    TagLib::APE::File file(&stream);
    TagLib::APE::Tag *tag = file.APETag();
    TagLib::APE::Properties *ap = file.audioProperties();

    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, ap->bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "Monkey's Audio");
        info->setDuration(ap->lengthInMilliseconds());
    }

    if((parts & TrackInfo::ReplayGainInfo) && tag && !tag->isEmpty())
    {
        TagLib::APE::ItemListMap items = tag->itemListMap();
        if(items.contains("REPLAYGAIN_TRACK_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN,TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].values()[0]));
        if(items.contains("REPLAYGAIN_TRACK_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK,TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].values()[0]));
        if(items.contains("REPLAYGAIN_ALBUM_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN,TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].values()[0]));
        if(items.contains("REPLAYGAIN_ALBUM_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK,TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].values()[0]));
    }

    if((parts & TrackInfo::MetaData) && tag && !tag->isEmpty())
    {
        if(tag->itemListMap().contains("CUESHEET"))
        {
            CueParser parser(tag->itemListMap()["CUESHEET"].toString().toCString(true));
            parser.setDuration(info->duration());
            parser.setProperties(info->properties());
            parser.setUrl("ape", filePath);
            delete info;
            return parser.createPlayList(track);
        }

        info->setValue(Qmmp::ALBUM, TStringToQString(tag->album()));
        info->setValue(Qmmp::ARTIST, TStringToQString(tag->artist()));
        info->setValue(Qmmp::COMMENT, TStringToQString(tag->comment()));
        info->setValue(Qmmp::GENRE, TStringToQString(tag->genre()));
        info->setValue(Qmmp::TITLE, TStringToQString(tag->title()));
        info->setValue(Qmmp::YEAR, tag->year());
        info->setValue(Qmmp::TRACK, tag->track());
        //additional metadata
        TagLib::APE::Item fld;
        if(!(fld = tag->itemListMap()["ALBUM ARTIST"]).isEmpty())
            info->setValue(Qmmp::ALBUMARTIST, TStringToQString(fld.toString()));
        if(!(fld = tag->itemListMap()["COMPOSER"]).isEmpty())
            info->setValue(Qmmp::COMPOSER, TStringToQString(fld.toString()));
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderFFapFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return new FFapMetaDataModel(path, readOnly);
}

void DecoderFFapFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ffap, DecoderFFapFactory)
#endif
