/***************************************************************************
 *   Copyright (C) 2008-2016 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#include <QtPlugin>

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#if (LIBAVUTIL_VERSION_INT >= ((51<<16)+(32<<8)+0))
#include <libavutil/dict.h>
#endif
}

#include "ffmpegmetadatamodel.h"
#include "decoder_ffmpeg.h"
#include "decoderffmpegfactory.h"


// DecoderFFmpegFactory

bool DecoderFFmpegFactory::supports(const QString &source) const
{
    foreach(QString filter, properties().filters)
    {
        QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
        if (regexp.exactMatch(source))
            return true;
    }
    return false;
}

bool DecoderFFmpegFactory::canDecode(QIODevice *i) const
{
    av_register_all();
    QStringList filters = properties().filters;

    AVProbeData  pd;
    uint8_t buf[PROBE_BUFFER_SIZE + AVPROBE_PADDING_SIZE];
    pd.filename = 0;
    pd.buf_size = i->peek((char*)buf, sizeof(buf) - AVPROBE_PADDING_SIZE);
    pd.buf = buf;
    if(pd.buf_size < PROBE_BUFFER_SIZE)
        return false;
    AVInputFormat *fmt = av_probe_input_format(&pd, 1);
    if(!fmt)
    {
        return false;
    }
    if(filters.contains("*.wma") && !memcmp(fmt->name, "asf", 3))
        return true;
    else if(filters.contains("*.mp3") && !memcmp(fmt->name, "mp3", 3))
        return true;
    else if(filters.contains("*.aac") && !memcmp(fmt->name, "aac", 3))
        return true;
    else if(filters.contains("*.ac3") && !memcmp(fmt->name, "eac3", 4))
        return true;
    else if(filters.contains("*.dts") && !memcmp(fmt->name, "dts", 3))
        return true;
    else if(filters.contains("*.mka") && !memcmp(fmt->name, "mka", 3))
        return true;
    else if(filters.contains("*.vqf") && !memcmp(fmt->name, "vqf", 3))
        return true;
    else if(filters.contains("*.ape") && !memcmp(fmt->name, "ape", 3))
        return true;
    else if(filters.contains("*.tta") && !memcmp(fmt->name, "tta", 3))
        return true;
    return false;
}

const DecoderProperties DecoderFFmpegFactory::properties() const
{
    av_register_all();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QStringList filters;
    filters << "*.wma" << "*.ape";
    filters = settings.value("FFMPEG_legacy/filters", filters).toStringList();

    if(!avcodec_find_decoder(CODEC_ID_AAC))
    {
        filters.removeAll("*.aac");
        filters.removeAll("*.m4a");
    }
    DecoderProperties properties;
    properties.name = tr("FFmpeg Plugin");
    properties.filters = filters;
    properties.description = tr("FFmpeg Formats");
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
    properties.shortName = "ffmpeg_legacy";
    properties.hasAbout = true;
    properties.hasSettings = true;
    properties.noInput = false;
    properties.priority = 10;
    return properties;
}

Decoder *DecoderFFmpegFactory::create(const QString &path, QIODevice *input)
{
    return new DecoderFFmpeg(path, input);
}

QList<FileInfo *> DecoderFFmpegFactory::createPlayList(const QString &fileName, bool useMetaData, QStringList *)
{

    QList <FileInfo*> list;
    avcodec_init();
    avcodec_register_all();
    av_register_all();
    AVFormatContext *in = 0;

    if (av_open_input_file(&in, fileName.toLocal8Bit(), 0, 0, 0) < 0)
    {
        qDebug("DecoderFFmpegFactory: unable to open file");
        return list;
    }
    FileInfo *info = new FileInfo(fileName);
    av_find_stream_info(in);

    if (useMetaData)
    {
        AVMetadataTag *album = av_metadata_get(in->metadata,"album",0,0);
        if(!album)
            album = av_metadata_get(in->metadata,"WM/AlbumTitle",0,0);
        AVMetadataTag *artist = av_metadata_get(in->metadata,"artist",0,0);
        if(!artist)
            artist = av_metadata_get(in->metadata,"author",0,0);
        AVMetadataTag *comment = av_metadata_get(in->metadata,"comment",0,0);
        AVMetadataTag *genre = av_metadata_get(in->metadata,"genre",0,0);
        AVMetadataTag *title = av_metadata_get(in->metadata,"title",0,0);
        AVMetadataTag *year = av_metadata_get(in->metadata,"WM/Year",0,0);
        if(!year)
            year = av_metadata_get(in->metadata,"year",0,0);
        if(!year)
            year = av_metadata_get(in->metadata,"date",0,0);
        AVMetadataTag *track = av_metadata_get(in->metadata,"track",0,0);
        if(!track)
            track = av_metadata_get(in->metadata,"WM/Track",0,0);
        if(!track)
            track = av_metadata_get(in->metadata,"WM/TrackNumber",0,0);

        if(album)
            info->setMetaData(Qmmp::ALBUM, QString::fromUtf8(album->value).trimmed());
        if(artist)
            info->setMetaData(Qmmp::ARTIST, QString::fromUtf8(artist->value).trimmed());
        if(comment)
            info->setMetaData(Qmmp::COMMENT, QString::fromUtf8(comment->value).trimmed());
        if(genre)
            info->setMetaData(Qmmp::GENRE, QString::fromUtf8(genre->value).trimmed());
        if(title)
            info->setMetaData(Qmmp::TITLE, QString::fromUtf8(title->value).trimmed());
        if(year)
            info->setMetaData(Qmmp::YEAR, year->value);
        if(track)
            info->setMetaData(Qmmp::TRACK, track->value);
    }
    info->setLength(in->duration/AV_TIME_BASE);
    av_close_input_file(in);
    list << info;
    return list;
}

MetaDataModel* DecoderFFmpegFactory::createMetaDataModel(const QString &path, QObject *parent)
{
    return new FFmpegMetaDataModel(path,parent);
}

Q_EXPORT_PLUGIN2(ffmpeg_legacy,DecoderFFmpegFactory)
