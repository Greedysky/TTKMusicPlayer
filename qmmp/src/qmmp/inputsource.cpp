#include <QDir>
#include <algorithm>
#include "qmmpplugincache_p.h"
#include "fileinputsource_p.h"
#include "emptyinputsource_p.h"
#include "inputsource.h"

InputSource::InputSource(const QString &source, QObject *parent)
    : QObject(parent),
      m_path(source)
{

}

bool InputSource::isWaiting() const
{
    return false;
}

QString InputSource::contentType() const
{
    return QString();
}

void InputSource::stop()
{

}

QString InputSource::path() const
{
    return m_path;
}

qint64 InputSource::offset() const
{
    return m_offset;
}

void InputSource::setOffset(qint64 offset)
{
    m_offset = offset;
}

bool InputSource::hasMetaData() const
{
    return m_hasMetaData;
}

QMap<Qmmp::MetaData, QString> InputSource::takeMetaData()
{
    m_hasMetaData = false;
    return m_metaData;
}

void InputSource::setProperty(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        m_properties.remove(key);
    else
        m_properties[key] = strValue;
}

void InputSource::setProperties(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(auto it = properties.begin(); it != properties.end(); ++it)
    {
        setProperty(it.key(), it.value());
    }
}

const QMap<Qmmp::TrackProperty, QString> &InputSource::properties() const
{
    return m_properties;
}

void InputSource::addMetaData(const QMap<Qmmp::MetaData, QString> &metaData)
{
    m_metaData = metaData;
    m_hasMetaData = true;
}

void InputSource::addStreamInfo(const QHash<QString, QString> &info)
{
    m_streamInfo = info;
    m_hasStreamInfo = true;
}

bool InputSource::hasStreamInfo() const
{
    return m_hasStreamInfo;
}

QHash<QString, QString> InputSource::takeStreamInfo()
{
    m_hasStreamInfo = false;
    return m_streamInfo;
}

// static methods
QStringList InputSource::m_disabledNames;
QList<QmmpPluginCache*> *InputSource::m_cache = nullptr;

InputSource *InputSource::create(const QString &url, QObject *parent)
{
    loadPlugins();
    if(!url.contains("://")) //local file path doesn't contain "://"
    {
        qDebug("InputSource: using file transport");
        return new FileInputSource(url, parent);
    }

    InputSourceFactory *factory = findByUrl(url);
    if(factory)
    {
        qDebug("InputSource: using %s transport", qPrintable(url.section("://", 0, 0)));
        return factory->create(url, parent);
    }
    else
    {
        qDebug("InputSource: using fake transport");
        return new EmptyInputSource(url, parent);
    }
}

QList<InputSourceFactory *> InputSource::factories()
{
    loadPlugins();
    QList<InputSourceFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->inputSourceFactory())
            list.append(item->inputSourceFactory());
    }
    return list;
}

QList<InputSourceFactory *> InputSource::enabledFactories()
{
    loadPlugins();
    QList<InputSourceFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->inputSourceFactory())
            list.append(item->inputSourceFactory());
    }
    return list;
}

QString InputSource::file(const InputSourceFactory *factory)
{
    loadPlugins();
    auto it = std::find_if(m_cache->constBegin(), m_cache->constEnd(),
                           [factory](QmmpPluginCache *item) { return item->shortName() == factory->properties().shortName; } );
    return it == m_cache->constEnd() ? QString() : (*it)->file();
}

QStringList InputSource::protocols()
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

QList<RegularExpression> InputSource::regExps()
{
    loadPlugins();
    QList<RegularExpression> regExpList;

    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->inputSourceFactory())
            regExpList << item->inputSourceFactory()->properties().regExps;
    }
    return regExpList;
}

InputSourceFactory *InputSource::findByUrl(const QString &url)
{
    loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        InputSourceFactory *factory = item->inputSourceFactory();
        if(!factory)
            continue;

        for(RegularExpression &r : factory->properties().regExps)
        {
            if(r.hasMatch(url))
                return factory;
        }
    }

    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        InputSourceFactory *factory = item->inputSourceFactory();

        if(factory && factory->properties().protocols.contains(url.section("://", 0, 0)))
            return factory;
    }

    return nullptr;
}

void InputSource::setEnabled(InputSourceFactory *factory, bool enable)
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
    settings.setValue("Transports/disabled_plugins", m_disabledNames);
}

bool InputSource::isEnabled(const InputSourceFactory *factory)
{
    loadPlugins();
    return !m_disabledNames.contains(factory->properties().shortName);
}

void InputSource::loadPlugins()
{
    if(m_cache)
        return;

    m_cache = new QList<QmmpPluginCache*>;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    for(const QString &filePath : Qmmp::findPlugins("Transports"))
    {
        QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    m_disabledNames = settings.value("Transports/disabled_plugins").toStringList();
    QmmpPluginCache::cleanup(&settings);
}
