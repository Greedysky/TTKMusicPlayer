#include "decoderffmpegfactory.h"
#include "ffmpegmetadatamodel.h"
#include "settingsdialog.h"
#include "decoder_ffmpeg.h"
#include "decoder_ffmpegcue.h"
#include "decoder_ffmpegm4b.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/avutil.h>
}
#include <QSettings>
#include <qmmp/cueparser.h>

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
    const QStringList &filters = properties().filters;

    AVProbeData pd;
    memset(&pd, 0, sizeof(pd));
    uint8_t buf[PROBE_BUFFER_SIZE + AVPROBE_PADDING_SIZE];
    pd.buf_size = input->peek((char*)buf, sizeof(buf) - AVPROBE_PADDING_SIZE);
    pd.buf = buf;

    if(pd.buf_size < PROBE_BUFFER_SIZE)
        return false;

    const AVInputFormat *fmt = av_probe_input_format(&pd, 1);
    if(!fmt)
        return false;

    const QStringList &formats = QString::fromLatin1(fmt->name).split(",");

    if(filters.contains("*.wma") && formats.contains("asf"))
        return true;
    else if(filters.contains("*.aac") && formats.contains("aac"))
        return true;
    else if(filters.contains("*.ac3") && formats.contains("eac3"))
        return true;
    else if(filters.contains("*.dts") && formats.contains("dts"))
        return true;
    else if(filters.contains("*.mka") && (formats.contains("mka") || formats.contains("matroska")))
        return true;
    else if(filters.contains("*.vqf") && formats.contains("vqf"))
        return true;
    else if(filters.contains("*.ape") && formats.contains("ape"))
        return true;
    else if(filters.contains("*.tta") && formats.contains("tta"))
        return true;
    else if(filters.contains("*.m4a") && (formats.contains("m4a") || formats.contains("mp4")))
        return true;
    else if(filters.contains("*.tak") && formats.contains("tak"))
        return true;
    else if(filters.contains("*.spx") && formats.contains("spx"))
        return true;
    else if(filters.contains("*.webm") && (formats.contains("webm") || formats.contains("matroska")))
        return true;
    else if(formats.contains("matroska") && avcodec_find_decoder(AV_CODEC_ID_OPUS) && input->isSequential()) //audio from YouTube
        return true;
    return false;
}

DecoderProperties DecoderFFmpegFactory::properties() const
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QStringList filters = {
        "*.wma", "*.ape", "*.tta", "*.m4a", "*.m4b", "*.aac", "*.ra", "*.shn", "*.vqf", "*.ac3", "*.tak", "*.spx", "*.webm", "*.dsf", "*.dsdiff", "*.mka"
    };

    const QStringList &disabledFilters = settings.value("FFMPEG/disabled_filters").toStringList();
    for(const QString &filter : qAsConst(disabledFilters))
        filters.removeAll(filter);

    if(filters.contains("*.m4a") && !filters.contains("*.m4b"))
        filters << "*.m4b";

    if(!avcodec_find_decoder(AV_CODEC_ID_WMAV1))
        filters.removeAll("*.wma");
    if(!avcodec_find_decoder(AV_CODEC_ID_APE))
        filters.removeAll("*.ape");
    if(!avcodec_find_decoder(AV_CODEC_ID_TTA))
        filters.removeAll("*.tta");
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC))
        filters.removeAll("*.aac");
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC) && !avcodec_find_decoder(AV_CODEC_ID_ALAC))
    {
        filters.removeAll("*.m4a");
        filters.removeAll("*.m4b");
    }
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
    if(!avcodec_find_decoder(AV_CODEC_ID_TRUESPEECH))
        filters.removeAll("*.spx");
    if(!avcodec_find_decoder(AV_CODEC_ID_OPUS))
        filters.removeAll("*.webm");
    if(!avcodec_find_decoder(AV_CODEC_ID_DSD_LSBF))
    {
        filters.removeAll("*.dsf");
        filters.removeAll("*.dsdiff");
    }

    DecoderProperties properties;
    properties.name = tr("FFmpeg Plugin");
    properties.shortName = "ffmpeg";
    properties.filters = filters;
    properties.description = "FFmpeg Formats File";

    if(filters.contains("*.wma"))
        properties.contentTypes << "audio/x-ms-wma";
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
    if(filters.contains("*.spx"))
        properties.contentTypes << "audio/speech";
    if(filters.contains("*.webm"))
        properties.contentTypes << "audio/webm";

    properties.protocols << "ffmpeg" << "m4b";
    properties.priority = 10;
    properties.hasSettings = true;
    return properties;
}

Decoder *DecoderFFmpegFactory::create(const QString &path, QIODevice *input)
{
    if(path.startsWith("ffmpeg://"))
        return new DecoderFFmpegCue(path);
    else if(path.startsWith("m4b://"))
        return new DecoderFFmpegM4b(this, path);
    else
        return new DecoderFFmpeg(path, input);
}

QList<TrackInfo*> DecoderFFmpegFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *ignoredPaths)
{
    int trackNumber = -1; //cue/m4b track
    QString filePath = path;

    if(path.contains("://")) //is it cue track?
    {
        filePath.remove("ffmpeg://");
        filePath.remove("m4b://");
        filePath.remove(RegularWrapper("#\\d+$"));
        trackNumber = path.section("#", -1).toInt();
        parts = TrackInfo::AllParts; //extract all metadata for single  cue/m4b track
    }
    else
    {
        if(ignoredPaths)
        {
            ignoredPaths->push_back(path);
        }
    }

    TrackInfo *info = new TrackInfo(filePath);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AVFormatContext *in = nullptr;
#ifdef Q_OS_WIN
    if(avformat_open_input(&in, qUtf8Printable(filePath), nullptr, nullptr) < 0)
#else
    if(avformat_open_input(&in, qPrintable(filePath), nullptr, nullptr) < 0)
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
            AVCodecParameters *c = in->streams[idx]->codecpar;
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
            return parser.createPlayList(trackNumber);
        }
        else if(trackNumber > 0 && path.startsWith("ffmpeg://")) //invalid track
        {
            avformat_close_input(&in);
            delete info;
            return QList<TrackInfo*>();
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

        if(in->nb_chapters > 1 && filePath.endsWith(".m4b", Qt::CaseInsensitive))
        {
            QList<TrackInfo*> playlist = createPlayListFromChapters(in, info, trackNumber);
            avformat_close_input(&in);
            delete info;
            return playlist;
        }
        else if(trackNumber > 0 && path.startsWith("m4b://")) //invalid chapter
        {
            avformat_close_input(&in);
            delete info;
            return QList<TrackInfo*>();
        }
    }

    avformat_close_input(&in);
    return QList<TrackInfo*>() << info;
}

QList<TrackInfo*> DecoderFFmpegFactory::createPlayListFromChapters(AVFormatContext *in, TrackInfo *extraInfo, int trackNumber)
{
    QList<TrackInfo*> playlist;
    for(unsigned int i = 0; i < in->nb_chapters; ++i)
    {
        if((trackNumber > 0) && (int(i + 1) != trackNumber))
            continue;

        AVChapter *chapter = in->chapters[i];
        TrackInfo *info = new TrackInfo(QString("m4b://%1#%2").arg(extraInfo->path()).arg(i + 1));
        info->setDuration((chapter->end - chapter->start) * av_q2d(chapter->time_base) * 1000);
        info->setValues(extraInfo->properties());
        info->setValues(extraInfo->metaData());
        info->setValue(Qmmp::TRACK, i + 1);

        AVDictionaryEntry *title = av_dict_get(chapter->metadata,"title", nullptr, 0);
        if(title)
            info->setValue(Qmmp::TITLE, QString::fromUtf8(title->value).trimmed());

        playlist << info;
    }

    return playlist;
}

MetaDataModel* DecoderFFmpegFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return new FFmpegMetaDataModel(path, readOnly);
}

void DecoderFFmpegFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ffmpeg, DecoderFFmpegFactory)
#endif
