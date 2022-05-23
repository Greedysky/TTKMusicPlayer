#include "format.h"
#include "decoder.h"
#include "decoderfactory.h"

bool Format::songTrackValid(const QString &url)
{
    QStringList list;
    list << "ape://";
    list << "cue://";
    list << "ffmpeg://";
    list << "flac://";
    list << "gme://";
    list << "m4b://";
    list << "sc68://";
    list << "sid://";
    list << "tfmx://";
#ifdef Q_OS_UNIX
    list << "uade://";
#endif
    list << "wvpack://";

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
