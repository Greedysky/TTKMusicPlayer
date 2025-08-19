#include "decodermpegfactory.h"
#include "mpegmetadatamodel.h"
#include "tagextractor.h"
#include "settingsdialog.h"
#include "decoder_mpg123.h"
#include <qmmp/qmmptextcodec.h>

#include <QSettings>
#include <taglib/apetag.h>
#include <taglib/textidentificationframe.h>

bool DecoderMPEGFactory::canDecode(QIODevice *input) const
{
    char buf[8192];
    qint64 dataSize = sizeof(buf);

    if(input->peek(buf, sizeof(buf)) != sizeof(buf))
        return false;

    if(!memcmp(buf, "FLV", 3)) //skip Macromedia Flash Video
        return false;

    if(!memcmp(buf + 8, "WAVE", 4))
        return !memcmp(buf + 20, "U" ,1);

    if(!memcmp(buf, "ID3", 3))
    {
        TagLib::ByteVector byteVector(buf, dataSize);
        TagLib::ID3v2::Header header(byteVector);

        //skip id3v2tag if possible
        if(input->isSequential())
        {
            if(header.tagSize() >= dataSize)
                return false;

            dataSize -= header.tagSize();
            memmove(buf, buf + header.tagSize(), dataSize);
        }
        else
        {
            input->seek(header.tagSize());
            dataSize = input->read(buf, sizeof(buf));
            input->seek(0); //restore initial position
        }
    }

    if(dataSize <= 0)
        return false;

    mpg123_init();
    mpg123_handle *handle = mpg123_new(nullptr, nullptr);
    if(!handle)
        return false;

    if(mpg123_open_feed(handle) != MPG123_OK)
    {
        mpg123_delete(handle);
        return false;
    }

    if(mpg123_format(handle, 44100, MPG123_STEREO, MPG123_ENC_SIGNED_16) != MPG123_OK)
    {
        mpg123_close(handle);
        mpg123_delete(handle);
        return false;
    }

    size_t out_size = 0;
    const int ret = mpg123_decode(handle, (unsigned char*) buf, dataSize, nullptr, 0, &out_size);
    mpg123_close(handle);
    mpg123_delete(handle);
    return ret == MPG123_DONE || ret == MPG123_NEW_FORMAT;
}

DecoderProperties DecoderMPEGFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("MPEG Plugin");
    properties.shortName = "mpeg";
    properties.filters << "*.mp1" << "*.mp2" << "*.mp3";
    properties.description = "MPEG Audio File";
    properties.contentTypes << "audio/mp3" << "audio/mpeg";
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderMPEGFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderMPG123(input);
}

QList<TrackInfo*> DecoderMPEGFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::FileStream stream(QStringToFileName(path), true);
#if TAGLIB_MAJOR_VERSION >= 2
    TagLib::MPEG::File fileRef(&stream);
#else
    TagLib::MPEG::File fileRef(&stream, TagLib::ID3v2::FrameFactory::instance());
#endif

    if(parts & TrackInfo::MetaData)
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        settings.beginGroup("MPEG");

        QList< QMap<Qmmp::MetaData, QString> > metaData;
        uint tag_array[3];
        tag_array[0] = settings.value("tag_1", SettingsDialog::ID3v2).toInt();
        tag_array[1] = settings.value("tag_2", SettingsDialog::APE).toInt();
        tag_array[2] = settings.value("tag_3", SettingsDialog::ID3v1).toInt();
        bool merge = settings.value("merge_tags", false).toBool();

        for(int i = 0; i < 3; ++i)
        {
            TagLib::Tag *tag = nullptr;
            QByteArray codecName;

            switch((uint) tag_array[i])
            {
            case SettingsDialog::ID3v1:
                codecName = settings.value("ID3v1_encoding", "GB18030").toByteArray();
                tag = fileRef.ID3v1Tag();
                break;
            case SettingsDialog::ID3v2:
                codecName = settings.value("ID3v2_encoding", "UTF-8").toByteArray();
                tag = fileRef.ID3v2Tag();
                break;
            case SettingsDialog::APE:
                codecName = "UTF-8";
                tag = fileRef.APETag();
                break;
            case SettingsDialog::Disabled:
                break;
            }

            if(codecName.contains("UTF") || codecName.isEmpty())
                codecName = "UTF-8";

            if(tag && !tag->isEmpty())
            {
                if((tag == fileRef.ID3v1Tag() || tag == fileRef.ID3v2Tag()) && settings.value("detect_encoding", false).toBool())
                {
                    QByteArray detectedCharset = TagExtractor::detectCharset(tag);
                    if(!detectedCharset.isEmpty())
                        codecName = detectedCharset;
                }

                QmmpTextCodec *codec = new QmmpTextCodec(codecName);
                const bool utf = codec->name().contains("UTF");

                QMap<Qmmp::MetaData, QString> tags;
                tags.insert(Qmmp::ARTIST, CSTR_TO_QSTR(codec, tag->artist(), utf));
                tags.insert(Qmmp::ALBUM, CSTR_TO_QSTR(codec, tag->album(), utf));
                tags.insert(Qmmp::COMMENT, CSTR_TO_QSTR(codec, tag->comment(), utf));
                tags.insert(Qmmp::GENRE, CSTR_TO_QSTR(codec, tag->genre(), utf));
                tags.insert(Qmmp::TITLE, CSTR_TO_QSTR(codec, tag->title(), utf));
                tags.insert(Qmmp::YEAR, QString::number(tag->year()));
                tags.insert(Qmmp::TRACK, QString::number(tag->track()));

                if(tag == fileRef.ID3v2Tag())
                {
                    if(!fileRef.ID3v2Tag()->frameListMap()["TPE2"].isEmpty())
                    {
                        TagLib::String albumArtist = fileRef.ID3v2Tag()->frameListMap()["TPE2"].front()->toString();
                        tags.insert(Qmmp::ALBUMARTIST, CSTR_TO_QSTR(codec, albumArtist, utf));
                    }
                    if(!fileRef.ID3v2Tag()->frameListMap()["TCOM"].isEmpty())
                    {
                        TagLib::String composer = fileRef.ID3v2Tag()->frameListMap()["TCOM"].front()->toString();
                        tags.insert(Qmmp::COMPOSER, CSTR_TO_QSTR(codec, composer, utf));
                    }
                    if(!fileRef.ID3v2Tag()->frameListMap()["TPOS"].isEmpty())
                    {
                        TagLib::String disc = fileRef.ID3v2Tag()->frameListMap()["TPOS"].front()->toString();
                        tags.insert(Qmmp::DISCNUMBER, CSTR_TO_QSTR(codec, disc, utf));
                    }
                }
                else if(tag == fileRef.APETag())
                {
                    TagLib::APE::Item fld;
                    if(!(fld = fileRef.APETag()->itemListMap()["ALBUM ARTIST"]).isEmpty())
                        tags.insert(Qmmp::ALBUMARTIST, CSTR_TO_QSTR(codec, fld.toString(), true));
                    if(!(fld = fileRef.APETag()->itemListMap()["COMPOSER"]).isEmpty())
                        tags.insert(Qmmp::COMPOSER, CSTR_TO_QSTR(codec, fld.toString(), true));
                }

                delete codec;
                metaData << tags;

                if(!merge)
                    break;
            }
        }
        settings.endGroup();

        for(const QMap<Qmmp::MetaData, QString> &tags : qAsConst(metaData))
        {
            for(int i = Qmmp::TITLE; i <= Qmmp::DISCNUMBER; ++i)
            {
                Qmmp::MetaData key = static_cast<Qmmp::MetaData>(i);
                if(info->value(key).length() < tags.value(key).length())
                    info->setValue(key, tags.value(key));
            }
        }
    }

    if((parts & TrackInfo::Properties) && fileRef.audioProperties())
    {
        info->setValue(Qmmp::BITRATE, fileRef.audioProperties()->bitrate());
        info->setValue(Qmmp::SAMPLERATE, fileRef.audioProperties()->sampleRate());
        info->setValue(Qmmp::CHANNELS, fileRef.audioProperties()->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, 32); //float
        switch(fileRef.audioProperties()->version())
        {
        case TagLib::MPEG::Header::Version1:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-1 layer %1").arg(fileRef.audioProperties()->layer()));
            break;
        case TagLib::MPEG::Header::Version2:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-2 layer %1").arg(fileRef.audioProperties()->layer()));
            break;
        case TagLib::MPEG::Header::Version2_5:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-2.5 layer %1").arg(fileRef.audioProperties()->layer()));
            break;
#if TAGLIB_MAJOR_VERSION >= 2
        case TagLib::MPEG::Header::Version4:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-4 layer %1").arg(fileRef.audioProperties()->layer()));
            break;
#endif
        }
#if TAGLIB_MAJOR_VERSION > 1 || (TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION >= 10)
        info->setDuration(fileRef.audioProperties()->lengthInMilliseconds());
#else
        info->setDuration(fileRef.audioProperties()->length() * 1000);
#endif
    }

    if(parts & TrackInfo::ReplayGainInfo)
    {
        if(fileRef.ID3v2Tag() && !fileRef.ID3v2Tag()->isEmpty())
        {
            TagLib::ID3v2::Tag *tag = fileRef.ID3v2Tag();
            TagLib::ID3v2::UserTextIdentificationFrame *frame = nullptr;
            TagLib::ID3v2::FrameList frames = tag->frameList("TXXX");
            for(TagLib::ID3v2::FrameList::Iterator it = frames.begin(); it != frames.end(); ++it)
            {
                frame = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame*>(*it);
                if(!frame || frame->fieldList().size() < 2)
                    continue;

                TagLib::String desc = frame->description().upper();
                if(desc == "REPLAYGAIN_TRACK_GAIN")
                    info->setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, TStringToQString(frame->fieldList()[1]));
                else if(desc == "REPLAYGAIN_TRACK_PEAK")
                    info->setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, TStringToQString(frame->fieldList()[1]));
                else if(desc == "REPLAYGAIN_ALBUM_GAIN")
                    info->setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, TStringToQString(frame->fieldList()[1]));
                else if(desc == "REPLAYGAIN_ALBUM_PEAK")
                    info->setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, TStringToQString(frame->fieldList()[1]));
            }
        }

        if(info->replayGainInfo().isEmpty() && fileRef.APETag() && !fileRef.APETag()->isEmpty())
        {
            TagLib::APE::Tag *tag = fileRef.APETag();
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
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderMPEGFactory::createMetaDataModel(const QString &path, bool readOnly)
{
   return new MPEGMetaDataModel(path, readOnly);
}

QDialog *DecoderMPEGFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mpeg, DecoderMPEGFactory)
#endif
