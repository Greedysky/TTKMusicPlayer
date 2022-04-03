#include "format.h"
#include "decoder.h"
#include "decoderfactory.h"

#define MUSIC_CUE_FILE              "cue"
#define MUSIC_APE_FILE              "ape"
#define MUSIC_FFMPEG_FILE           "ffmpeg"
#define MUSIC_M4B_FILE              "m4b"
#define MUSIC_FLAC_FILE             "flac"
#define MUSIC_GME_FILE              "gme"
#define MUSIC_SID_FILE              "sid"
#define MUSIC_WVPACK_FILE           "wvpack"
#define MUSIC_SC68_FILE             "sc68"

bool Format::songTrackValid(const QString &url)
{
    QStringList list;
    list << MUSIC_CUE_FILE "://";
    list << MUSIC_APE_FILE "://";
    list << MUSIC_FFMPEG_FILE "://";
    list << MUSIC_M4B_FILE "://";
    list << MUSIC_FLAC_FILE "://";
    list << MUSIC_GME_FILE "://";
    list << MUSIC_SID_FILE "://";
    list << MUSIC_WVPACK_FILE "://";
    list << MUSIC_SC68_FILE "://";

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

QMap<QString, QStringList> Format::supportMusicMapFormats()
{
    QMap<QString, QStringList> formats;
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        QStringList filters;
        for(QString filter : factory->properties().filters)
        {
            filters << filter.remove(0, 2); // remove *.
        }

        if(!filters.isEmpty())
        {
            formats.insert(factory->properties().shortName, filters);
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
