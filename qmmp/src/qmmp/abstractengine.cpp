#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include "enginefactory.h"
#include "qmmpaudioengine_p.h"
#include "qmmpplugincache_p.h"
#include "statehandler.h"
#include "abstractengine.h"

AbstractEngine::AbstractEngine(QObject *parent)
    : QThread(parent)
{

}

AbstractEngine::~AbstractEngine()
{

}

QMutex *AbstractEngine::mutex()
{
    return &m_mutex;
}

// static methods
QStringList AbstractEngine::m_disabledNames;
QList<QmmpPluginCache*> *AbstractEngine::m_cache = nullptr;

void AbstractEngine::loadPlugins()
{
    if(m_cache)
        return;

    m_cache = new QList<QmmpPluginCache*>;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    for(const QString &filePath : Qmmp::findPlugins("Engines"))
    {
        QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    m_disabledNames = settings.value("Engine/disabled_plugins").toStringList();
    QmmpPluginCache::cleanup(&settings);
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


    loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
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
    loadPlugins();
    QList<EngineFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->engineFactory())
            list.append(item->engineFactory());
    }
    return list;
}

QList<EngineFactory *> AbstractEngine::enabledFactories()
{
    loadPlugins();
    QList<EngineFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->engineFactory())
            list.append(item->engineFactory());
    }
    return list;
}

QStringList AbstractEngine::nameFilters()
{
    loadPlugins();
    QStringList filters;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        filters << item->filters();
    }
    return filters;
}

QStringList AbstractEngine::contentTypes()
{
    loadPlugins();
    QStringList types;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        types << item->contentTypes();
    }
    return types;
}

EngineFactory *AbstractEngine::findByFilePath(const QString &source)
{
    loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        EngineFactory *fact = item->engineFactory();
        if(fact && fact->supports(source))
            return fact;
    }
    return nullptr;
}

void AbstractEngine::setEnabled(EngineFactory *factory, bool enable)
{
    loadPlugins();
    if(!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
        m_disabledNames.removeAll(factory->properties().shortName);
    else
        m_disabledNames.append(factory->properties().shortName);

    m_disabledNames.removeDuplicates();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Engine/disabled_plugins", m_disabledNames);
}

bool AbstractEngine::isEnabled(const EngineFactory *factory)
{
    loadPlugins();
    return !m_disabledNames.contains(factory->properties().shortName);
}

bool AbstractEngine::isEnabled(const AbstractEngine *engine)
{
    if(engine->objectName().isEmpty()) //qmmp engine
        return true;

    loadPlugins();
    return !m_disabledNames.contains(engine->objectName());
}

QString AbstractEngine::file(const EngineFactory *factory)
{
    loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList AbstractEngine::protocols()
{
    loadPlugins();
    QStringList protocolList;

    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        protocolList << item->protocols();
    }
    protocolList.removeDuplicates();
    return protocolList;
}
