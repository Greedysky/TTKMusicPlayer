#include <QSettings>
#include "qmmpaudioengine_p.h"
#include "qmmpplugincache_p.h"
#include "abstractengine.h"

class AbstractEnginePrivate
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
        QVariantHash priorities = settings.value("Engine/priorities").toHash();
        for(const QString &filePath : Qmmp::findPlugins("Engines"))
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

        disabledNames = settings.value("Engine/disabled_plugins").toStringList();
        std::stable_sort(cache->begin(), cache->end(), _pluginCacheLessComparator);
        QmmpPluginCache::cleanup(&settings);
        qAddPostRoutine(AbstractEnginePrivate::cleanup);
    }

    static void cleanup()
    {
        if(cache)
        {
            qDeleteAll(*cache);
        }
    }

    QMutex mutex;

    static QList<QmmpPluginCache*> *cache;
    static QStringList disabledNames;

};

QStringList AbstractEnginePrivate::disabledNames;
QList<QmmpPluginCache*> *AbstractEnginePrivate::cache = nullptr;


AbstractEngine::AbstractEngine(QObject *parent)
    : QThread(parent),
      d(new AbstractEnginePrivate)
{

}

AbstractEngine::~AbstractEngine()
{
    delete d;
}

QMutex *AbstractEngine::mutex()
{
    return &d->mutex;
}

AbstractEngine *AbstractEngine::create(InputSource *s, QObject *parent)
{
    AbstractEngine *engine = new QmmpAudioEngine(parent); //internal engine
    if(!engine->enqueue(s))
    {
        engine->deleteLater();
        engine = nullptr;
    }
    else
        return engine;

    AbstractEnginePrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;
        EngineFactory *fact = item->engineFactory();
        if(!fact)
            continue;
        engine = fact->create(parent); //engine plugin
        engine->setObjectName(item->shortName());
        if(!engine->enqueue(s))
        {
            engine->deleteLater();
            engine = nullptr;
        }
        else
            break;
    }
    return engine;
}

QList<EngineFactory *> AbstractEngine::factories()
{
    AbstractEnginePrivate::loadPlugins();
    QList<EngineFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(item->engineFactory())
            list.append(item->engineFactory());
    }
    return list;
}

QList<EngineFactory *> AbstractEngine::enabledFactories()
{
    AbstractEnginePrivate::loadPlugins();
    QList<EngineFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;
        if(item->engineFactory())
            list.append(item->engineFactory());
    }
    return list;
}

QStringList AbstractEngine::nameFilters()
{
    AbstractEnginePrivate::loadPlugins();
    QStringList filters;
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;

        filters << item->filters();
    }
    return filters;
}

QStringList AbstractEngine::contentTypes()
{
    AbstractEnginePrivate::loadPlugins();
    QStringList types;
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;

        types << item->contentTypes();
    }
    return types;
}

EngineFactory *AbstractEngine::findByFilePath(const QString &source)
{
    AbstractEnginePrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;
        EngineFactory *fact = item->engineFactory();
        if(fact && fact->supports(source))
            return fact;
    }
    return nullptr;
}

void AbstractEngine::setEnabled(EngineFactory *factory, bool enable)
{
    AbstractEnginePrivate::loadPlugins();
    if(!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
        AbstractEnginePrivate::disabledNames.removeAll(factory->properties().shortName);
    else
        AbstractEnginePrivate::disabledNames.append(factory->properties().shortName);

    AbstractEnginePrivate::disabledNames.removeDuplicates();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Engine/disabled_plugins", AbstractEnginePrivate::disabledNames);
}

bool AbstractEngine::isEnabled(const EngineFactory *factory)
{
    AbstractEnginePrivate::loadPlugins();
    return !AbstractEnginePrivate::disabledNames.contains(factory->properties().shortName);
}

bool AbstractEngine::isEnabled(const AbstractEngine *engine)
{
    if(engine->objectName().isEmpty()) //qmmp engine
        return true;

    AbstractEnginePrivate::loadPlugins();
    return !AbstractEnginePrivate::disabledNames.contains(engine->objectName());
}

QString AbstractEngine::file(const EngineFactory *factory)
{
    AbstractEnginePrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList AbstractEngine::protocols()
{
    AbstractEnginePrivate::loadPlugins();
    QStringList protocolList;

    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(AbstractEnginePrivate::disabledNames.contains(item->shortName()))
            continue;

        protocolList << item->protocols();
    }
    protocolList.removeDuplicates();
    return protocolList;
}

void AbstractEngine::setPriority(const EngineFactory *factory, int priority)
{
    AbstractEnginePrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
        {
            item->setPriority(priority);
            QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
            QVariantHash priorities = settings.value("Engine/priorities").toHash();
            priorities.insert(item->shortName(), priority);
            settings.setValue("Engine/priorities", priorities);
            std::stable_sort(AbstractEnginePrivate::cache->begin(), AbstractEnginePrivate::cache->end(), AbstractEnginePrivate::_pluginCacheLessComparator);
            break;
        }
    }
}

int AbstractEngine::priority(const EngineFactory *factory)
{
    AbstractEnginePrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*AbstractEnginePrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->priority();
    }
    return 0;
}
