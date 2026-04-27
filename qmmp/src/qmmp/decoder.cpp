#include <QDir>
#include "qmmpplugincache_p.h"
#include "decoderfactory.h"

class DecoderPrivate
{
public:
    //sort cache items by priority
    static bool _pluginCacheLessComparator(const QmmpPluginCache* f1, const QmmpPluginCache* f2)
    {
        return f1->priority() < f2->priority();
    }

    static void loadPlugins()
    {
        if(cache)
            return;

        cache = new QList<QmmpPluginCache*>;
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        QVariantHash priorities = settings.value("Decoder/priorities").toHash();
        for(const QString &filePath : Qmmp::findPlugins("Input"))
        {
            QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
            if(item->hasError())
            {
                delete item;
                continue;
            }
            item->setPriority(priorities.value(item->shortName(), item->priority()).toInt());
            cache->append(item);
        }

        disabledNames = settings.value("Decoder/disabled_plugins").toStringList();
        std::stable_sort(cache->begin(), cache->end(), _pluginCacheLessComparator);
        QmmpPluginCache::cleanup(&settings);
        qAddPostRoutine(DecoderPrivate::cleanup);
    }

    static void cleanup()
    {
        if(cache)
        {
            QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
            for(QmmpPluginCache *item : qAsConst(*cache))
            {
                item->update(&settings);
            }
            qDeleteAll(*cache);
        }
    }

    AudioParameters parameters;
    QMap<Qmmp::TrackProperty, QString> properties;
    QIODevice *input;
    bool hasMetaData = false;
    QMap<Qmmp::MetaData, QString> metaData;
    QMap<Qmmp::ReplayGainKey, double> replayGain; //replay gain information

    static QList<QmmpPluginCache*> *cache;
    static QStringList disabledNames;

};

QStringList DecoderPrivate::disabledNames;
QList<QmmpPluginCache*> *DecoderPrivate::cache = nullptr;


Decoder::Decoder(QIODevice *input)
    : d(new DecoderPrivate)
{
    d->input = input;
}

Decoder::~Decoder()
{
    delete d;
}

void Decoder::setReplayGainInfo(const QMap<Qmmp::ReplayGainKey, double> &rg)
{
    d->replayGain = rg;
}

void Decoder::configure(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat format)
{
    configure(AudioParameters(srate, map, format));
}

void Decoder::configure(quint32 srate, int channels, Qmmp::AudioFormat f)
{
    qDebug("Decoder: using internal channel order");
    configure(AudioParameters(srate, ChannelMap(channels), f));
}

void Decoder::configure(const AudioParameters &p)
{
    d->parameters = p;
    setProperty(Qmmp::SAMPLERATE, d->parameters.sampleRate());
    setProperty(Qmmp::CHANNELS, d->parameters.channels());
    setProperty(Qmmp::BITS_PER_SAMPLE, d->parameters.validBitsPerSample());
}

void Decoder::next()
{

}

QString Decoder::nextURL() const
{
    return QString();
}

const AudioParameters &Decoder::audioParameters() const
{
    return d->parameters;
}

const QMap<Qmmp::ReplayGainKey, double> &Decoder::replayGainInfo() const
{
    return d->replayGain;
}

void Decoder::addMetaData(const QMap<Qmmp::MetaData, QString> &metaData)
{
    d->metaData = metaData;
    d->hasMetaData = true;
}

QIODevice *Decoder::input() const
{
    return d->input;
}

bool Decoder::hasMetaData() const
{
    return d->hasMetaData;
}

QMap<Qmmp::MetaData, QString> Decoder::takeMetaData()
{
    d->hasMetaData = false;
    return d->metaData;
}

void Decoder::setProperty(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        d->properties.remove(key);
    else
        d->properties[key] = strValue;
}

void Decoder::setProperties(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(auto it = properties.begin(); it != properties.end(); ++it)
    {
        setProperty(it.key(), it.value());
    }
}

QMap<Qmmp::TrackProperty, QString> Decoder::properties() const
{
    return d->properties;
}

QString Decoder::file(const DecoderFactory *factory)
{
    DecoderPrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList Decoder::protocols()
{
    DecoderPrivate::loadPlugins();
    QStringList protocolList;

    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        protocolList << item->protocols();
    }
    protocolList.removeDuplicates();
    return protocolList;
}

DecoderFactory *Decoder::findByFilePath(const QString &path, bool useContent)
{
    DecoderPrivate::loadPlugins();
    //get list of available/supported factories
    QList<DecoderFactory*> filtered = useContent ? enabledFactories() : findByFileExtension(path);

    if(filtered.isEmpty())
        return nullptr;

    //try to find by content
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Decoder: file open error: %s", qPrintable(file.errorString()));
        return nullptr;
    }

    for(DecoderFactory *fact : qAsConst(filtered))
    {
        if(fact->canDecode(&file))
            return fact;
    }

    //fallback: try to find by extension
    if(useContent)
        filtered = findByFileExtension(path);

    for(DecoderFactory *fact : qAsConst(filtered))
    {
        if(fact->properties().noInput || fact->properties().protocols.contains("file"))
            return fact;
    }

    //fallback: try to find by content
    if(!useContent)
        return findByContent(&file);

    //fallback: use first available factory
    if(!filtered.isEmpty() && !useContent)
         return filtered.first();

    return nullptr;
}

DecoderFactory *Decoder::findByMime(const QString &type)
{
    if(type.isEmpty())
        return nullptr;

    DecoderPrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;
        DecoderFactory *fact = item->decoderFactory();
        if(fact && !fact->properties().noInput && fact->properties().contentTypes.contains(type))
            return fact;
    }
    return nullptr;
}

DecoderFactory *Decoder::findByContent(QIODevice *input)
{
    DecoderPrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;
        DecoderFactory *fact = item->decoderFactory();
        if(fact && !fact->properties().noInput && fact->canDecode(input))
            return fact;
    }
    return nullptr;
}

DecoderFactory *Decoder::findByProtocol(const QString &p)
{
    DecoderPrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        if(item->decoderFactory() && item->decoderFactory()->properties().protocols.contains(p))
            return item->decoderFactory();
    }
    return nullptr;
}

QList<DecoderFactory *> Decoder::findByFileExtension(const QString &path)
{
    QList<DecoderFactory*> filtered;
    DecoderFactory *fact = nullptr;
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        if(!(fact = item->decoderFactory()))
            continue;

        if(QDir::match(fact->properties().filters, path.section(QChar('/'), -1)))
            filtered.append(fact);
    }

    return filtered;
}

void Decoder::setEnabled(DecoderFactory *factory, bool enable)
{
    DecoderPrivate::loadPlugins();
    if(!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
        DecoderPrivate::disabledNames.removeAll(factory->properties().shortName);
    else
        DecoderPrivate::disabledNames.append(factory->properties().shortName);

    DecoderPrivate::disabledNames.removeDuplicates();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Decoder/disabled_plugins", DecoderPrivate::disabledNames);
}

bool Decoder::isEnabled(const DecoderFactory *factory)
{
    DecoderPrivate::loadPlugins();
    return !DecoderPrivate::disabledNames.contains(factory->properties().shortName);
}

void Decoder::setPriority(const DecoderFactory *factory, int priority)
{
    DecoderPrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
        {
            item->setPriority(priority);
            QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
            QVariantHash priorities = settings.value("Decoder/priorities").toHash();
            priorities.insert(item->shortName(), priority);
            settings.setValue("Decoder/priorities", priorities);
            std::stable_sort(DecoderPrivate::cache->begin(), DecoderPrivate::cache->end(), DecoderPrivate::_pluginCacheLessComparator);
            break;
        }
    }
}

int Decoder::priority(const DecoderFactory *factory)
{
    DecoderPrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->priority();
    }
    return 0;
}

QList<DecoderFactory *> Decoder::factories()
{
    DecoderPrivate::loadPlugins();
    QList<DecoderFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(item->decoderFactory())
            list.append(item->decoderFactory());
    }
    return list;
}

QList<DecoderFactory *> Decoder::enabledFactories()
{
    DecoderPrivate::loadPlugins();
    QList<DecoderFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        if(item->decoderFactory())
            list.append(item->decoderFactory());
    }
    return list;
}

QStringList Decoder::nameFilters()
{
    DecoderPrivate::loadPlugins();
    QStringList filters;
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        filters << item->filters();
    }
    return filters;
}

QStringList Decoder::contentTypes()
{
    DecoderPrivate::loadPlugins();
    QStringList types;
    for(QmmpPluginCache *item : qAsConst(*DecoderPrivate::cache))
    {
        if(DecoderPrivate::disabledNames.contains(item->shortName()))
            continue;

        types << item->contentTypes();
    }
    return types;
}
