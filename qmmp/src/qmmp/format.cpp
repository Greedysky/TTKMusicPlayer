#include "format.h"
#include "decoder.h"
#include "decoderfactory.h"

bool Format::isTrack(const QString &url)
{
    QStringList list;
    list << "ape://";
    list << "cue://";
    list << "fc14://";
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

bool Format::isRedirection(const QString &url)
{
    QStringList list;
    list << "cue://";

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
            if(filter.startsWith("*."))
            {
                formats << filter.remove(0, 2); // remove first *.
            }
            else
            {
                filter.chop(2); // remove last .*
                formats << filter;
            }
        }
    }

    formats.removeDuplicates();
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

    formats.removeDuplicates();
    return formats;
}

QStringList Format::supportMusicInputFormats()
{
    QString allFilters;
    QStringList formats;
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        const QString &filters = factory->properties().filters.join(" ");
        if(filters.isEmpty())
        {
            continue;
        }

        allFilters += " " + filters;
        formats << QString("%1(%2)").arg(factory->properties().description, filters);
    }

    formats.insert(0, QString("All Files(%1)").arg(allFilters));
    return formats;
}
