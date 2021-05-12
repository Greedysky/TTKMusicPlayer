#include "format.h"
#include "decoder.h"
#include "decoderfactory.h"

QStringList Format::decodeForString()
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

QMap<QString, QStringList> Format::decodeForStringMap()
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

QStringList Format::decodeForFilter()
{
    QStringList formats;
    const QList<DecoderFactory *> factorys(Decoder::enabledFactories());
    for(DecoderFactory *factory : qAsConst(factorys))
    {
        formats << factory->properties().filters;
    }
    return formats;
}

QStringList Format::decodeForFilterDialog()
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
