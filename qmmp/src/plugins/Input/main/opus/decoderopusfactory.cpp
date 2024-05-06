#include "decoderopusfactory.h"
#include "decoder_opus.h"
#include "opusmetadatamodel.h"

bool DecoderOpusFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if(input->peek(buf, 36) != 36)
        return false;
    return !memcmp(buf, "OggS", 4) && !memcmp(buf + 28, "OpusHead", 8);
}

DecoderProperties DecoderOpusFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Opus Plugin");
    properties.shortName = "opus";
    properties.filters << "*.opus";
    properties.description = "Ogg Opus File";
    properties.contentTypes << "audio/opus";
    return properties;
}

Decoder *DecoderOpusFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderOpus(input);
}

QList<TrackInfo*> DecoderOpusFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::Ogg::Opus::File fileRef(QStringToFileName(path));
    if((parts & TrackInfo::MetaData) && fileRef.tag() && !fileRef.tag()->isEmpty())
    {
        TagLib::Ogg::XiphComment *tag = fileRef.tag();
        info->setValue(Qmmp::ALBUM, TStringToQString(tag->album()));
        info->setValue(Qmmp::ARTIST, TStringToQString(tag->artist()));
        info->setValue(Qmmp::COMMENT, TStringToQString(tag->comment()));
        info->setValue(Qmmp::GENRE, TStringToQString(tag->genre()));
        info->setValue(Qmmp::TITLE, TStringToQString(tag->title()));
        info->setValue(Qmmp::YEAR, tag->year());
        info->setValue(Qmmp::TRACK, tag->track());

        const TagLib::Ogg::FieldListMap &items = tag->fieldListMap();
        if(!items["ALBUMARTIST"].isEmpty())
            info->setValue(Qmmp::ALBUMARTIST, TStringToQString(items["ALBUMARTIST"].toString()));
        if(!items["COMPOSER"].isEmpty())
            info->setValue(Qmmp::COMPOSER, TStringToQString(items["COMPOSER"].toString()));
        if(!items["DISCNUMBER"].isEmpty())
            info->setValue(Qmmp::DISCNUMBER, TStringToQString(items["DISCNUMBER"].toString()));
    }

    TagLib::Ogg::Opus::Properties *ap = fileRef.audioProperties();
    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, 32); //float
        info->setValue(Qmmp::FORMAT_NAME, "Ogg Opus");
        info->setDuration(ap->lengthInMilliseconds());
    }

    if((parts & TrackInfo::ReplayGainInfo) && fileRef.tag() && !fileRef.tag()->isEmpty())
    {
        TagLib::Ogg::XiphComment *tag = fileRef.tag();
        const TagLib::Ogg::FieldListMap &items = tag->fieldListMap();
        if(items.contains("REPLAYGAIN_TRACK_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, TStringToQString(items["REPLAYGAIN_TRACK_GAIN"].front()));
        if(items.contains("REPLAYGAIN_TRACK_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, TStringToQString(items["REPLAYGAIN_TRACK_PEAK"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_GAIN"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, TStringToQString(items["REPLAYGAIN_ALBUM_GAIN"].front()));
        if(items.contains("REPLAYGAIN_ALBUM_PEAK"))
            info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, TStringToQString(items["REPLAYGAIN_ALBUM_PEAK"].front()));
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderOpusFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return new OpusMetaDataModel(path, readOnly);
}

QDialog *DecoderOpusFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(opus, DecoderOpusFactory)
#endif
