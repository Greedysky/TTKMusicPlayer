#include "qmmpaudioengine_p.h"
#include "qmmpplugincache_p.h"
#include "effect.h"

class EffectPrivate
{
public:
    static bool _effectCacheCompareFunc(QmmpPluginCache *e1, QmmpPluginCache *e2)
    {
        return e1->priority() > e2->priority();
    }

    static void loadPlugins()
    {
        if(cache)
            return;

        cache = new QList<QmmpPluginCache *>;
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        for(const QString &filePath : Qmmp::findPlugins("Effect"))
        {
            QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
            if(item->hasError())
            {
                delete item;
                continue;
            }
            cache->append(item);
        }

        std::stable_sort(cache->begin(), cache->end(), _effectCacheCompareFunc);
        enabledNames = settings.value("Effect/enabled_plugins").toStringList();
        qAddPostRoutine(EffectPrivate::cleanup);
    }

    static void cleanup()
    {
        if(cache)
        {
            qDeleteAll(*cache);
        }
    }

    EffectFactory *factory = nullptr;
    quint32 freq = 0;
    int channels = 0;
    ChannelMap chanMap;

    static QList<QmmpPluginCache*> *cache;
    static QStringList enabledNames;

};

QList<QmmpPluginCache*> *EffectPrivate::cache = nullptr;
QStringList EffectPrivate::enabledNames;


Effect::Effect()
    : d(new EffectPrivate)
{

}

Effect::~Effect()
{
    delete d;
}

void Effect::configure(quint32 freq, ChannelMap map)
{
    d->freq = freq;
    d->chanMap = map;
    d->channels = map.count();
}

quint32 Effect::sampleRate() const
{
    return d->freq;
}

int Effect::channels() const
{
    return d->channels;
}

const ChannelMap &Effect::channelMap() const
{
    return d->chanMap;
}

AudioParameters Effect::audioParameters() const
{
    return AudioParameters(d->freq, d->chanMap, Qmmp::PCM_FLOAT);
}

EffectFactory* Effect::factory() const
{
    return d->factory;
}

Effect* Effect::create(EffectFactory *factory)
{
    EffectPrivate::loadPlugins();
    Effect *effect = factory->create();
    effect->d->factory = factory;
    return effect;
}

QList<EffectFactory *> Effect::factories()
{
    EffectPrivate::loadPlugins();
    QList<EffectFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*EffectPrivate::cache))
    {
        if(item->effectFactory())
            list.append(item->effectFactory());
    }
    return list;
}

QList<EffectFactory *> Effect::enabledFactories()
{
    EffectPrivate::loadPlugins();
    QList<EffectFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*EffectPrivate::cache))
    {
        if(EffectPrivate::enabledNames.contains(item->shortName()) && item->effectFactory())
            list.append(item->effectFactory());
    }
    return list;
}

QString Effect::file(const EffectFactory *factory)
{
    EffectPrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*EffectPrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

void Effect::setEnabled(EffectFactory *factory, bool enable)
{
    EffectPrivate::loadPlugins();
    if(!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
    {
        if(QmmpAudioEngine::instance())
            QmmpAudioEngine::instance()->addEffect(factory);
        EffectPrivate::enabledNames.append(factory->properties().shortName);
    }
    else
    {
        EffectPrivate::enabledNames.removeAll(factory->properties().shortName);
        if(QmmpAudioEngine::instance())
            QmmpAudioEngine::instance()->removeEffect(factory);
    }

    EffectPrivate::enabledNames.removeDuplicates();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Effect/enabled_plugins", EffectPrivate::enabledNames);
    QmmpPluginCache::cleanup(&settings);
}

bool Effect::isEnabled(const EffectFactory *factory)
{
    EffectPrivate::loadPlugins();
    return EffectPrivate::enabledNames.contains(factory->properties().shortName);
}

EffectFactory *Effect::findFactory(const QString &shortName)
{
    EffectPrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*EffectPrivate::cache))
    {
        if(item->shortName() == shortName)
            return item->effectFactory();
    }
    return nullptr;
}
