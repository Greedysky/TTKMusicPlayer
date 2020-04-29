/***************************************************************************
 *   Copyright (C) 2008-2020 by Ilya Kotov                                 *
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
#include <qmmp/cueparser.h>
extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/avutil.h>
}

#include "ffmpegmetadatamodel.h"
#include "decoder_ffmpeg.h"
#include "decoder_ffmpegcue.h"
#include "decoderffmpegfactory.h"

DecoderFFmpegFactory::DecoderFFmpegFactory()
{
#if (LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58,10,100)) //ffmpeg-3.5
    avcodec_register_all();
    avformat_network_init();
    av_register_all();
#endif
}

bool DecoderFFmpegFactory::canDecode(QIODevice *input) const
{
    QStringList filters = properties().filters;

    AVProbeData pd;
    memset(&pd, 0, sizeof(pd));
    uint8_t buf[PROBE_BUFFER_SIZE + AVPROBE_PADDING_SIZE];
    pd.buf_size = input->peek((char*)buf, sizeof(buf) - AVPROBE_PADDING_SIZE);
    pd.buf = buf;
    if(pd.buf_size < PROBE_BUFFER_SIZE)
        return false;
    AVInputFormat *fmt = av_probe_input_format(&pd, 1);
    if(!fmt)
        return false;

    QStringList formats = QString::fromLatin1(fmt->name).split(",");

    if(filters.contains("*.wma") && formats.contains("asf"))
        return true;
    else if(filters.contains("*.mp3") && formats.contains("mp3"))
        return true;
    else if(filters.contains("*.aac") && formats.contains("aac"))
        return true;
    else if(filters.contains("*.ac3") && formats.contains("eac3"))
        return true;
    else if(filters.contains("*.dts") && formats.contains("dts"))
        return true;
    else if(filters.contains("*.mka") && formats.contains("mka"))
        return true;
    else if(filters.contains("*.vqf") && formats.contains("vqf"))
        return true;
    else if(filters.contains("*.ape") && formats.contains("ape"))
        return true;
    else if(filters.contains("*.tta") && formats.contains("tta"))
        return true;
    else if(filters.contains("*.m4a") && (formats.contains("m4a") || formats.contains("mp4")))
        return true;
    else if(filters.contains("*.tak"))
        return true;
    return false;
}

DecoderProperties DecoderFFmpegFactory::properties() const
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QStringList filters = {
        "*.wma", "*.ape", "*.tta", "*.m4a", "*.aac", "*.ra", "*.shn", "*.vqf", "*.ac3", "*.tak", "*.dsf", "*.dsdiff"
    };
    filters = settings.value("FFMPEG/filters", filters).toStringList();

    if(!avcodec_find_decoder(AV_CODEC_ID_WMAV1))
        filters.removeAll("*.wma");
    if(!avcodec_find_decoder(AV_CODEC_ID_APE))
        filters.removeAll("*.ape");
    if(!avcodec_find_decoder(AV_CODEC_ID_TTA))
        filters.removeAll("*.tta");
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC))
        filters.removeAll("*.aac");
    if(!avcodec_find_decoder(AV_CODEC_ID_MP3))
        filters.removeAll("*.mp3");
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC) && !avcodec_find_decoder(AV_CODEC_ID_ALAC))
        filters.removeAll("*.m4a");
    if(!avcodec_find_decoder(AV_CODEC_ID_RA_288))
        filters.removeAll("*.ra");
    if(!avcodec_find_decoder(AV_CODEC_ID_SHORTEN))
        filters.removeAll("*.shn");
    if(!avcodec_find_decoder(AV_CODEC_ID_EAC3))
        filters.removeAll("*.ac3");
    if(!avcodec_find_decoder(AV_CODEC_ID_DTS))
        filters.removeAll("*.dts");
    if(!avcodec_find_decoder(AV_CODEC_ID_TRUEHD))
        filters.removeAll("*.mka");
    if(!avcodec_find_decoder(AV_CODEC_ID_TWINVQ))
        filters.removeAll("*.vqf");
    if(!avcodec_find_decoder(AV_CODEC_ID_TAK))
        filters.removeAll("*.tak");
    if(!avcodec_find_decoder(AV_CODEC_ID_DSD_LSBF))
    {
        filters.removeAll("*.dsf");
        filters.removeAll("*.dsdiff");
    }

    DecoderProperties properties;
    properties.name = tr("FFmpeg Plugin");
    properties.filters = filters;
    properties.description = tr("FFmpeg Formats Files");
    if(filters.contains("*.wma"))
        properties.contentTypes << "audio/x-ms-wma";
    if(filters.contains("*.mp3"))
        properties.contentTypes << "audio/mpeg";
    if(filters.contains("*.aac"))
        properties.contentTypes << "audio/aac" << "audio/aacp";
    if(filters.contains("*.shn"))
        properties.contentTypes << "audio/x-ffmpeg-shorten";
    if(filters.contains("*.m4a"))
    {
        properties.contentTypes << "audio/3gpp" << "audio/3gpp2" << "audio/mp4";
        properties.contentTypes << "audio/MP4A-LATM" << "audio/mpeg4-generic";
        properties.contentTypes << "audio/m4a";
    }
    if(filters.contains("*.ac3"))
        properties.contentTypes << "audio/ac3" << "audio/eac3";
    if(filters.contains("*.dts"))
        properties.contentTypes << "audio/dts";
    if(filters.contains("*.mka"))
        properties.contentTypes << "audio/true-hd" << "audio/x-matroska";
    properties.shortName = "ffmpeg";
    properties.hasSettings = true;
    properties.protocols << "ffmpeg";
    properties.priority = 10;
    return properties;
}

Decoder *DecoderFFmpegFactory::create(const QString &path, QIODevice *input)
{
    if(path.startsWith("ffmpeg://"))
        return new DecoderFFmpegCue(path);
    else
        return new DecoderFFmpeg(path, input);
}

QList<TrackInfo*> DecoderFFmpegFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    int track = -1; //cue track
    QString filePath = path;

    if(path.contains("://")) //is it cue track?
    {
        filePath.remove("ffmpeg://");
        filePath.remove(QRegExp("#\\d+$"));
        track = path.section("#", -1).toInt();
        parts = TrackInfo::AllParts; //extract all metadata for single cue track
    }

     TrackInfo *info = new TrackInfo(filePath);

    if(parts == TrackInfo::Parts())
        return QList<TrackInfo*>() << info;

    AVFormatContext *in = nullptr;

#ifdef Q_OS_WIN
    if(avformat_open_input(&in, filePath.toUtf8().constData(), nullptr, nullptr) < 0)
#else
    if(avformat_open_input(&in, filePath.toLocal8Bit().constData(), nullptr, nullptr) < 0)
#endif
    {
        qDebug("DecoderFFmpegFactory: unable to open file");
        delete info;
        return  QList<TrackInfo*>();
    }

    avformat_find_stream_info(in, nullptr);

    if(parts & TrackInfo::Properties)
    {
        int idx = av_find_best_stream(in, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
        if(idx >= 0)
        {
    #if (LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(57,48,0)) //ffmpeg-3.1:  57.48.101
            AVCodecParameters *c = in->streams[idx]->codecpar;
    #else
            AVCodecContext *c = in->streams[idx]->codec;
    #endif
            info->setValue(Qmmp::BITRATE, int(c->bit_rate) / 1000);
            info->setValue(Qmmp::SAMPLERATE, c->sample_rate);
            info->setValue(Qmmp::CHANNELS, c->channels);
            info->setValue(Qmmp::BITS_PER_SAMPLE, c->bits_per_raw_sample);

            AVCodec *codec = avcodec_find_decoder(c->codec_id);
            if(codec)
                info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(codec->name));
            info->setValue(Qmmp::FILE_SIZE, QFileInfo(filePath).size()); //adds file size for cue tracks
            info->setDuration(in->duration * 1000 / AV_TIME_BASE);
        }
    }

    if(parts & TrackInfo::MetaData)
    {
        AVDictionaryEntry *cuesheet = av_dict_get(in->metadata,"cuesheet",nullptr,0);
        if(cuesheet)
        {
            CueParser parser(cuesheet->value);
            parser.setDuration(info->duration());
            parser.setProperties(info->properties());
            parser.setUrl("ffmpeg", filePath);

            avformat_close_input(&in);
            delete info;
            return (track > 0) ? parser.createPlayList(track) : parser.createPlayList();
        }

        AVDictionaryEntry *album = av_dict_get(in->metadata,"album",nullptr,0);
        AVDictionaryEntry *album_artist = av_dict_get(in->metadata,"album_artist",nullptr,0);
        AVDictionaryEntry *artist = av_dict_get(in->metadata,"artist",nullptr,0);
        AVDictionaryEntry *composer = av_dict_get(in->metadata,"composer",nullptr,0);
        AVDictionaryEntry *comment = av_dict_get(in->metadata,"comment",nullptr,0);
        AVDictionaryEntry *genre = av_dict_get(in->metadata,"genre",nullptr,0);
        AVDictionaryEntry *title = av_dict_get(in->metadata,"title",nullptr,0);
        AVDictionaryEntry *year = av_dict_get(in->metadata,"date",nullptr,0);
        AVDictionaryEntry *track = av_dict_get(in->metadata,"track",nullptr,0);

        if(!album)
            album = av_dict_get(in->metadata,"WM/AlbumTitle",nullptr,0);

        if(!artist)
            artist = av_dict_get(in->metadata,"author",nullptr,0);

        if(!year)
            year = av_dict_get(in->metadata,"WM/Year",nullptr,0);
        if(!year)
            year = av_dict_get(in->metadata,"year",nullptr,0);

        if(!track)
            track = av_dict_get(in->metadata,"WM/Track",nullptr,0);
        if(!track)
            track = av_dict_get(in->metadata,"WM/TrackNumber",nullptr,0);

        if(album)
            info->setValue(Qmmp::ALBUM, QString::fromUtf8(album->value).trimmed());
        if(album_artist)
            info->setValue(Qmmp::ALBUMARTIST, QString::fromUtf8(album_artist->value).trimmed());
        if(artist)
            info->setValue(Qmmp::ARTIST, QString::fromUtf8(artist->value).trimmed());
        if(composer)
            info->setValue(Qmmp::COMPOSER, QString::fromUtf8(composer->value).trimmed());
        if(comment)
            info->setValue(Qmmp::COMMENT, QString::fromUtf8(comment->value).trimmed());
        if(genre)
            info->setValue(Qmmp::GENRE, QString::fromUtf8(genre->value).trimmed());
        if(title)
            info->setValue(Qmmp::TITLE, QString::fromUtf8(title->value).trimmed());
        if(year)
            info->setValue(Qmmp::YEAR, year->value);
        if(track)
            info->setValue(Qmmp::TRACK, track->value);
    }

    avformat_close_input(&in);
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderFFmpegFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new FFmpegMetaDataModel(path);
}


