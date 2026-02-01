#include "decoderffmpegfactory.h"
#include "ffmpegmetadatamodel.h"
#include "decoder_ffmpeg.h"
#include "decoder_ffmpegcue.h"
#include "decoder_ffmpegm4b.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/avutil.h>
}
#include <qmmp/cueparser.h>

DecoderFFmpegFactory::DecoderFFmpegFactory()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 10, 100) //ffmpeg-3.5
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

    if((filters.contains("*.wma") || filters.contains("*.asf")) && formats.contains("asf"))
        return true;
    else if(filters.contains("*.ape") && formats.contains("ape"))
        return true;
    else if(filters.contains("*.tta") && formats.contains("tta"))
        return true;
    else if((filters.contains("*.mov") || filters.contains("*.mp4") || filters.contains("*.m4a") || filters.contains("*.3gp") || filters.contains("*.3g2") ||
             filters.contains("*.f4v") || filters.contains("*.hevc") || filters.contains("*.m4v")) &&
            (formats.contains("mov") || formats.contains("mp4") || formats.contains("m4a") || formats.contains("3gp") || formats.contains("3g2")))
        return true;
    else if(filters.contains("*.aac") && formats.contains("aac"))
        return true;
    else if(filters.contains("*.ra") && formats.contains("rm"))
        return true;
    else if(filters.contains("*.shn") && formats.contains("shn"))
        return true;
    else if(filters.contains("*.ac3") && (formats.contains("ac3") || formats.contains("eac3")))
        return true;
    else if(filters.contains("*.dts") && formats.contains("dts"))
        return true;
    else if(filters.contains("*.mka") && (formats.contains("mka") || formats.contains("matroska")))
        return true;
    else if(filters.contains("*.vqf") && formats.contains("vqf"))
        return true;
    else if(filters.contains("*.tak") && formats.contains("tak"))
        return true;
    else if(filters.contains("*.dsf") && formats.contains("dsf"))
        return true;
    else if(filters.contains("*.spx") && (formats.contains("spx") || formats.contains("ogg")))
        return true;
    else if(filters.contains("*.adx") && formats.contains("adx"))
        return true;
    else if(filters.contains("*.aix") && formats.contains("aix"))
        return true;
    else if(filters.contains("*.wve") && formats.contains("wve"))
        return true;
    else if(filters.contains("*.sln") && formats.contains("sln"))
        return true;
    else if(filters.contains("*.paf") && formats.contains("epaf"))
        return true;
    else if(filters.contains("*.pvf") && formats.contains("pvf"))
        return true;
    else if(filters.contains("*.ircam") && formats.contains("ircam"))
        return true;
    else if(filters.contains("*.gsm") && formats.contains("gsm"))
        return true;
    else if(filters.contains("*.avr") && formats.contains("avr"))
        return true;
    else if(filters.contains("*.amr") && formats.contains("amr"))
        return true;
    else if(filters.contains("*.webm") && (formats.contains("webm") || formats.contains("matroska")))
        return true;
    else if(filters.contains("*.avi") && formats.contains("avi"))
        return true;
    else if(filters.contains("*.flv") && formats.contains("flv"))
        return true;
    else if((filters.contains("*.mpeg") || filters.contains("*.vob")) && formats.contains("mpeg"))
        return true;
    else if(filters.contains("*.rm") && formats.contains("rm"))
        return true;
    else if(filters.contains("*.swf") && formats.contains("swf"))
        return true;
    else if(filters.contains("*.wtv") && formats.contains("wtv"))
        return true;
    else if(formats.contains("matroska") && avcodec_find_decoder(AV_CODEC_ID_OPUS) && input->isSequential()) //audio from YouTube
        return true;
    return false;
}

DecoderProperties DecoderFFmpegFactory::properties() const
{
    QStringList filters = {
        "*.wma", "*.ape", "*.tta", "*.m4a", "*.m4b", "*.aac", "*.ra", "*.shn", "*.ac3", "*.mka", "*.vqf", "*.tak", "*.spx", "*.adx", "*.aix",
        "*.wve", "*.sln", "*.paf", "*.pvf", "*.ircam", "*.gsm", "*.avr", "*.amr", "*.dsf", "*.dsdiff",
        "*.webm", "*.3g2", "*.3gp", "*.asf", "*.avi", "*.f4v", "*.flv", "*.hevc", "*.m4v", "*.mov", "*.mp4", "*.mpeg", "*.rm", "*.swf", "*.vob", "*.wtv"
    };

    if(filters.contains("*.m4a") && !filters.contains("*.m4b"))
        filters << "*.m4b";

    if(!avcodec_find_decoder(AV_CODEC_ID_WMAV1))
        filters.removeAll("*.wma");
    if(!avcodec_find_decoder(AV_CODEC_ID_APE))
        filters.removeAll("*.ape");
    if(!avcodec_find_decoder(AV_CODEC_ID_TTA))
        filters.removeAll("*.tta");
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC) && !avcodec_find_decoder(AV_CODEC_ID_ALAC))
    {
        filters.removeAll("*.m4a");
        filters.removeAll("*.m4b");
        filters.removeAll("*.m4v");
        filters.removeAll("*.mov");
        filters.removeAll("*.mp4");
        filters.removeAll("*.f4v");
        filters.removeAll("*.flv");
        filters.removeAll("*.hevc");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_AAC))
        filters.removeAll("*.aac");
    if(!avcodec_find_decoder(AV_CODEC_ID_RA_288))
        filters.removeAll("*.ra");
    if(!avcodec_find_decoder(AV_CODEC_ID_SHORTEN))
        filters.removeAll("*.shn");
    if(!avcodec_find_decoder(AV_CODEC_ID_AC3) && !avcodec_find_decoder(AV_CODEC_ID_EAC3))
        filters.removeAll("*.ac3");
    if(!avcodec_find_decoder(AV_CODEC_ID_DTS))
        filters.removeAll("*.dts");
    if(!avcodec_find_decoder(AV_CODEC_ID_TRUEHD))
        filters.removeAll("*.mka");
    if(!avcodec_find_decoder(AV_CODEC_ID_TWINVQ))
        filters.removeAll("*.vqf");
    if(!avcodec_find_decoder(AV_CODEC_ID_TAK))
        filters.removeAll("*.tak");
    if(!avcodec_find_decoder(AV_CODEC_ID_SPEEX))
        filters.removeAll("*.spx");
    if(!avcodec_find_decoder(AV_CODEC_ID_ADPCM_ADX))
    {
        filters.removeAll("*.adx");
        filters.removeAll("*.aix");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_PCM_ALAW))
        filters.removeAll("*.wve");
    if(!avcodec_find_decoder(AV_CODEC_ID_PCM_S16LE))
    {
        filters.removeAll("*.sln");
        filters.removeAll("*.ircam");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_PCM_S16BE))
    {
        filters.removeAll("*.paf");
        filters.removeAll("*.pvf");
        filters.removeAll("*.avr");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_GSM))
        filters.removeAll("*.gsm");
    if(!avcodec_find_decoder(AV_CODEC_ID_AMR_NB))
    {
        filters.removeAll("*.amr");
        filters.removeAll("*.3g2");
        filters.removeAll("*.3gp");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_OPUS))
        filters.removeAll("*.webm");
    if(!avcodec_find_decoder(AV_CODEC_ID_WMAV2))
        filters.removeAll("*.asf");
    if(!avcodec_find_decoder(AV_CODEC_ID_MP3))
    {
        filters.removeAll("*.avi");
        filters.removeAll("*.swf");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_MP2))
    {
        filters.removeAll("*.mpeg");
        filters.removeAll("*.vob");
    }
    if(!avcodec_find_decoder(AV_CODEC_ID_AC3))
    {
        filters.removeAll("*.rm");
        filters.removeAll("*.wtv");
    }

    DecoderProperties properties;
    properties.name = tr("FFmpeg Plugin");
    properties.shortName = "ffmpeg";
    properties.filters = filters;
    properties.description = "FFmpeg Formats File";

    if(filters.contains("*.wma"))
        properties.contentTypes << "audio/x-ms-wma";
    if(filters.contains("*.m4a"))
    {
        properties.contentTypes << "audio/3gpp" << "audio/3gpp2" << "audio/mp4";
        properties.contentTypes << "audio/MP4A-LATM" << "audio/mpeg4-generic";
        properties.contentTypes << "audio/m4a";
    }
    if(filters.contains("*.aac"))
        properties.contentTypes << "audio/aac" << "audio/aacp";
    if(filters.contains("*.ra"))
        properties.contentTypes << "audio/vnd.rn-realaudio";
    if(filters.contains("*.shn"))
        properties.contentTypes << "audio/x-ffmpeg-shorten";
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

QList<TrackInfo*> DecoderFFmpegFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    int trackNumber = -1; //cue/m4b track
    QString filePath = path;

    if(path.contains("://")) //is it cue track?
    {
        filePath = TrackInfo::pathFromUrl(path, &trackNumber);
        parts = TrackInfo::AllParts; //extract all metadata for single  cue/m4b track
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
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(59, 37, 100) //ffmpeg-5.1
            info->setValue(Qmmp::CHANNELS, c->ch_layout.nb_channels);
#else
            info->setValue(Qmmp::CHANNELS, c->channels);
#endif
            if(c->bits_per_raw_sample > 0)
                info->setValue(Qmmp::BITS_PER_SAMPLE, c->bits_per_raw_sample);
            else
                info->setValue(Qmmp::BITS_PER_SAMPLE, av_get_bytes_per_sample(static_cast<AVSampleFormat>(c->format)) * 8);

            AVCodec *codec = avcodec_find_decoder(c->codec_id);
            if(codec)
                info->setValue(Qmmp::FORMAT_NAME, QString::fromLatin1(codec->name));
            else
                info->setValue(Qmmp::FORMAT_NAME, "FFMPEG");
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
    for(unsigned int i = 1; i <= in->nb_chapters; ++i)
    {
        if((trackNumber > 0) && (int(i) != trackNumber))
            continue;

        AVChapter *chapter = in->chapters[i - 1];
        TrackInfo *info = new TrackInfo(QString("m4b://%1#%2").arg(extraInfo->path()).arg(i));
        info->setDuration((chapter->end - chapter->start) * av_q2d(chapter->time_base) * 1000);
        info->setValues(extraInfo->properties());
        info->setValues(extraInfo->metaData());
        info->setValue(Qmmp::TRACK, i);

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

QDialog *DecoderFFmpegFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(ffmpeg, DecoderFFmpegFactory)
#endif
