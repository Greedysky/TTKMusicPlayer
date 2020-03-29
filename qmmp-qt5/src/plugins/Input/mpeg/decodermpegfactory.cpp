/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QSettings>
#include <QFile>
#include <QTextCodec>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/apetag.h>
#include <taglib/tfile.h>
#include <taglib/mpegfile.h>
#include <taglib/tfilestream.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2header.h>
#include <taglib/textidentificationframe.h>
#include <taglib/id3v2framefactory.h>
#include "mpegmetadatamodel.h"
#ifdef WITH_MAD
#include "decoder_mad.h"
#endif
#ifdef WITH_MPG123
#include "decoder_mpg123.h"
#endif
#include "decodermpegfactory.h"

#define CSTR_TO_QSTR(str,utf) codec->toUnicode(str.toCString(utf)).trimmed()

DecoderMPEGFactory::DecoderMPEGFactory()
{
    //detecting rusxmms patch
    m_using_rusxmms = false;
    char str[] = { char(0xF2), char(0xE5), char(0xF1), char(0xF2), '\0'};
    QTextCodec *codec = QTextCodec::codecForName ("windows-1251");
    TagLib::String tstr(str);
    if(codec->toUnicode(str) == QString::fromUtf8(tstr.toCString(true)))
    {
        qDebug("DecoderMADFactory: found taglib with rusxmms patch");
        m_using_rusxmms = true;
    }
}

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

    QString decoderName;
#if defined(WITH_MAD) && defined(WITH_MPG123)
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    decoderName = settings.value("MPEG/decoder", "mad").toString();
#elif defined(WITH_MAD)
    decoderName = "mad";
#elif defined(WITH_MPG123)
    decoderName = "mpg123";
#endif

#ifdef WITH_MAD
    if(decoderName != "mpg123")
    {
        struct mad_stream stream;
        struct mad_header header;
        struct mad_frame frame;
        int dec_res;

        mad_stream_init (&stream);
        mad_header_init (&header);
        mad_frame_init(&frame);
        mad_stream_buffer (&stream, (unsigned char *) buf, dataSize);
        stream.error = MAD_ERROR_NONE;

        while ((dec_res = mad_header_decode(&header, &stream)) == -1
               && MAD_RECOVERABLE(stream.error))
            ;

        if(dec_res == 0)
            dec_res = mad_frame_decode(&frame, &stream);

        mad_stream_finish(&stream);
        mad_frame_finish(&frame);
        return dec_res == 0;
    }
#endif

#ifdef WITH_MPG123
    if(decoderName == "mpg123")
    {
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
        int ret = mpg123_decode(handle, (unsigned char*) buf, dataSize, nullptr, 0, &out_size);
        mpg123_close(handle);
        mpg123_delete(handle);
        return ret == MPG123_DONE || ret == MPG123_NEW_FORMAT;
    }
#endif
    return false;
}

DecoderProperties DecoderMPEGFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("MPEG Plugin");
    properties.shortName = "mpeg";
    properties.filters << "*.mp1" << "*.mp2" << "*.mp3" << "*.wav";
    properties.description = tr("MPEG Audio Files");
    properties.contentTypes << "audio/mp3" << "audio/mpeg";
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderMPEGFactory::create(const QString &, QIODevice *input)
{
    Decoder *d = nullptr;
#if defined(WITH_MAD) && defined(WITH_MPG123)
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    if(settings.value("MPEG/decoder", "mad").toString() == "mpg123")
    {
        qDebug("DecoderMPEGFactory: using mpg123 decoder");
        d = new DecoderMPG123(input);
    }
    else
    {
        qDebug("DecoderMPEGFactory: using MAD decoder");
        d = new DecoderMAD(input);
    }
#elif defined(WITH_MAD)
    d = new DecoderMAD(input);
#elif defined(WITH_MPG123)
    d = new DecoderMPG123(input);
#endif
    return d;
}

QList<TrackInfo *> DecoderMPEGFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
        return QList<TrackInfo *>() << info;

    TagLib::Tag *tag = nullptr;
    TagLib::FileStream stream(QStringToFileName(path), true);
    TagLib::MPEG::File fileRef(&stream, TagLib::ID3v2::FrameFactory::instance());

    if(parts & TrackInfo::MetaData)
    {
//        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
//        settings.beginGroup("MPEG");

        QTextCodec *codec = nullptr;

//        uint tag_array[3];
//        tag_array[0] = settings.value("tag_1", SettingsDialog::ID3v2).toInt();
//        tag_array[1] = settings.value("tag_2", SettingsDialog::APE).toInt();
//        tag_array[2] = settings.value("tag_3", SettingsDialog::ID3v1).toInt();

//        QByteArray codecName;
//        for(int i = 0; i < 3; ++i)
//        {
//            codecName.clear();
//            switch ((uint) tag_array[i])
//            {
//            case SettingsDialog::ID3v1:
//                codecName = settings.value("ID3v1_encoding","ISO-8859-1").toByteArray();
//                tag = fileRef.ID3v1Tag();
//                break;
//            case SettingsDialog::ID3v2:
//                codecName = settings.value("ID3v2_encoding","UTF-8").toByteArray ();
                tag = fileRef.ID3v2Tag();
//                break;
//            case SettingsDialog::APE:
//                codecName = "UTF-8";
//                tag = fileRef.APETag();
//                break;
//            case SettingsDialog::Disabled:
//                break;
//            }
//            if(tag && !tag->isEmpty())
//            {
//                if(codecName.contains("UTF"))
//                    codecName = "UTF-8";
//                if(!codecName.isEmpty())
//                    codec = QTextCodec::codecForName(codecName);
//                break;
//            }
//        }
//        settings.endGroup();

        if(m_using_rusxmms)
            codec = QTextCodec::codecForName("UTF-8");

        if(!codec)
            codec = QTextCodec::codecForName("UTF-8");

        if(tag && codec)
        {
            bool utf = codec->name().contains("UTF");
            info->setValue(Qmmp::ALBUM, CSTR_TO_QSTR(tag->album(), utf));
            info->setValue(Qmmp::ARTIST, CSTR_TO_QSTR(tag->artist(), utf));
            info->setValue(Qmmp::COMMENT, CSTR_TO_QSTR(tag->comment(), utf));
            info->setValue(Qmmp::GENRE, CSTR_TO_QSTR(tag->genre(), utf));
            info->setValue(Qmmp::TITLE, CSTR_TO_QSTR(tag->title(), utf));
            info->setValue(Qmmp::YEAR, tag->year());
            info->setValue(Qmmp::TRACK, tag->track());

            if(tag == fileRef.ID3v2Tag())
            {
                if(!fileRef.ID3v2Tag()->frameListMap()["TPE2"].isEmpty())
                {
                    TagLib::String albumArtist;
                    albumArtist = fileRef.ID3v2Tag()->frameListMap()["TPE2"].front()->toString();
                    info->setValue(Qmmp::ALBUMARTIST, CSTR_TO_QSTR(albumArtist, utf));
                }
                if(!fileRef.ID3v2Tag()->frameListMap()["TCOM"].isEmpty())
                {
                    TagLib::String composer;
                    composer = fileRef.ID3v2Tag()->frameListMap()["TCOM"].front()->toString();
                    info->setValue(Qmmp::COMPOSER, CSTR_TO_QSTR(composer, utf));
                }
                if(!fileRef.ID3v2Tag()->frameListMap()["TPOS"].isEmpty())
                {
                    TagLib::String disc = fileRef.ID3v2Tag()->frameListMap()["TPOS"].front()->toString();
                    info->setValue(Qmmp::DISCNUMBER, CSTR_TO_QSTR(disc, utf));
                }
            }
            else if(tag == fileRef.APETag())
            {
                TagLib::APE::Item fld;
                if(!(fld = fileRef.APETag()->itemListMap()["ALBUM ARTIST"]).isEmpty())
                    info->setValue(Qmmp::ALBUMARTIST, CSTR_TO_QSTR(fld.toString(), true));
                if(!(fld = fileRef.APETag()->itemListMap()["COMPOSER"]).isEmpty())
                    info->setValue(Qmmp::COMPOSER, CSTR_TO_QSTR(fld.toString(), true));
            }
        }
    }

    TagLib::MPEG::Properties *ap = fileRef.audioProperties();
    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, 32); //float
        switch(ap->version())
        {
        case TagLib::MPEG::Header::Version1:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-1 layer %1").arg(ap->layer()));
            break;
        case TagLib::MPEG::Header::Version2:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-2 layer %1").arg(ap->layer()));
            break;
        case TagLib::MPEG::Header::Version2_5:
            info->setValue(Qmmp::FORMAT_NAME, QString("MPEG-2.5 layer %1").arg(ap->layer()));
        }
        info->setDuration(ap->lengthInMilliseconds());
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

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderMPEGFactory::createMetaDataModel(const QString &path, bool readOnly)
{
   return new MPEGMetaDataModel(m_using_rusxmms, path, readOnly);
}
