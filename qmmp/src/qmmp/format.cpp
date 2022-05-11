#include "format.h"
#include "decoder.h"
#include "decoderfactory.h"

#define MUSIC_APE_FILE              "ape"
#define MUSIC_CUE_FILE              "cue"
#define MUSIC_FFMPEG_FILE           "ffmpeg"
#define MUSIC_FLAC_FILE             "flac"
#define MUSIC_GME_FILE              "gme"
#define MUSIC_M4B_FILE              "m4b"
#define MUSIC_SC68_FILE             "sc68"
#define MUSIC_SID_FILE              "sid"
#define MUSIC_TFMX_FILE             "tfmx"
#define MUSIC_UADE_FILE             "uade"
#define MUSIC_WVPACK_FILE           "wvpack"

bool Format::songTrackValid(const QString &url)
{
    QStringList list;
    list << MUSIC_APE_FILE "://";
    list << MUSIC_CUE_FILE "://";
    list << MUSIC_FFMPEG_FILE "://";
    list << MUSIC_FLAC_FILE "://";
    list << MUSIC_GME_FILE "://";
    list << MUSIC_M4B_FILE "://";
    list << MUSIC_SC68_FILE "://";
    list << MUSIC_SID_FILE "://";
    list << MUSIC_TFMX_FILE "://";
    list << MUSIC_UADE_FILE "://";
    list << MUSIC_WVPACK_FILE "://";

    for(const QString &path : qAsConst(list))
    {
        if(url.startsWith(path))
        {
            return true;
        }
    }
    return false;
}

QStringList Format::supportMusicFormats()
{
    QStringList formats;
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        for(QString filter : factory->properties().filters)
        {
            formats << filter.remove(0, 2); // remove *.
        }
    }
    return formats;
}

QStringList Format::supportMusicInputFilterFormats()
{
    QStringList formats;
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        formats << factory->properties().filters;
    }
    return formats;
}

QStringList Format::supportMusicInputFormats()
{
    QStringList formats("All File(*.*)");
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        const QString &filters = factory->properties().filters.join(" ");
        if(filters.isEmpty())
        {
            continue;
        }

        formats << QString("%1(%2)").arg(factory->properties().description).arg(filters);
    }
    return formats;
}
