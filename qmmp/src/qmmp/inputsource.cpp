#include "qmmpplugincache_p.h"
#include "fileinputsource_p.h"
#include "emptyinputsource_p.h"
#include "inputsource.h"

#include <QCoreApplication>

class InputSourcePrivate
{
public:
    InputSourcePrivate(const QString &source)
        : path(source)
    {

    }

    static void loadPlugins()
    {
        if(cache)
            return;

        cache = new QList<QmmpPluginCache*>;
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        for(const QString &filePath : Qmmp::findPlugins("Transports"))
        {
            QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
            if(item->hasError())
            {
                delete item;
                continue;
            }
            cache->append(item);
        }

        disabledNames = settings.value("Transports/disabled_plugins").toStringList();
        QmmpPluginCache::cleanup(&settings);
        qAddPostRoutine(InputSourcePrivate::cleanup);
    }

    static void cleanup()
    {
        if(cache)
        {
            qDeleteAll(*cache);
        }
    }

    QString path;
    qint64 offset = -1;
    QMap<Qmmp::MetaData, QString> metaData;
    QMap<Qmmp::TrackProperty, QString> properties;
    QHash<QString, QString> streamInfo;
    bool hasMetaData = false, hasStreamInfo = false;

    static QList<QmmpPluginCache*> *cache;
    static QStringList disabledNames;

};

QStringList InputSourcePrivate::disabledNames;
QList<QmmpPluginCache*> *InputSourcePrivate::cache = nullptr;


InputSource::InputSource(const QString &source, QObject *parent)
    : QObject(parent),
      d(new InputSourcePrivate(source))
{

}

InputSource::~InputSource()
{
    delete d;
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
    return d->path;
}

qint64 InputSource::offset() const
{
    return d->offset;
}

void InputSource::setOffset(qint64 offset)
{
    d->offset = offset;
}

bool InputSource::hasMetaData() const
{
    return d->hasMetaData;
}

QMap<Qmmp::MetaData, QString> InputSource::takeMetaData()
{
    d->hasMetaData = false;
    return d->metaData;
}

void InputSource::setProperty(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        d->properties.remove(key);
    else
        d->properties[key] = strValue;
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
    return d->properties;
}

void InputSource::addMetaData(const QMap<Qmmp::MetaData, QString> &metaData)
{
    d->metaData = metaData;
    d->hasMetaData = true;
}

void InputSource::addStreamInfo(const QHash<QString, QString> &info)
{
    d->streamInfo = info;
    d->hasStreamInfo = true;
}

bool InputSource::hasStreamInfo() const
{
    return d->hasStreamInfo;
}

QHash<QString, QString> InputSource::takeStreamInfo()
{
    d->hasStreamInfo = false;
    return d->streamInfo;
}

InputSource *InputSource::create(const QString &url, QObject *parent)
{
    InputSourcePrivate::loadPlugins();
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

    qDebug("InputSource: using fake transport");
    return new EmptyInputSource(url, parent);
}

QList<InputSourceFactory *> InputSource::factories()
{
    InputSourcePrivate::loadPlugins();
    QList<InputSourceFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(item->inputSourceFactory())
            list.append(item->inputSourceFactory());
    }
    return list;
}

QList<InputSourceFactory *> InputSource::enabledFactories()
{
    InputSourcePrivate::loadPlugins();
    QList<InputSourceFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(InputSourcePrivate::disabledNames.contains(item->shortName()))
            continue;
        if(item->inputSourceFactory())
            list.append(item->inputSourceFactory());
    }
    return list;
}

QString InputSource::file(const InputSourceFactory *factory)
{
    InputSourcePrivate::loadPlugins();
    auto it = std::find_if(InputSourcePrivate::cache->constBegin(), InputSourcePrivate::cache->constEnd(),
                           [factory](QmmpPluginCache *item) { return item->shortName() == factory->properties().shortName; } );
    return it == InputSourcePrivate::cache->constEnd() ? QString() : (*it)->file();
}

QStringList InputSource::protocols()
{
    InputSourcePrivate::loadPlugins();
    QStringList protocolList;

    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(InputSourcePrivate::disabledNames.contains(item->shortName()))
            continue;

        protocolList << item->protocols();
    }
    protocolList.removeDuplicates();
    return protocolList;
}

QList<RegularExpression> InputSource::regExps()
{
    InputSourcePrivate::loadPlugins();
    QList<RegularExpression> regExpList;

    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(InputSourcePrivate::disabledNames.contains(item->shortName()))
            continue;
        if(item->inputSourceFactory())
            regExpList << item->inputSourceFactory()->properties().regExps;
    }
    return regExpList;
}

InputSourceFactory *InputSource::findByUrl(const QString &url)
{
    InputSourcePrivate::loadPlugins();
    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(InputSourcePrivate::disabledNames.contains(item->shortName()))
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

    for(QmmpPluginCache *item : qAsConst(*InputSourcePrivate::cache))
    {
        if(InputSourcePrivate::disabledNames.contains(item->shortName()))
            continue;

        InputSourceFactory *factory = item->inputSourceFactory();

        if(factory && factory->properties().protocols.contains(url.section("://", 0, 0)))
            return factory;
    }

    return nullptr;
}

void InputSource::setEnabled(InputSourceFactory *factory, bool enable)
{
    InputSourcePrivate::loadPlugins();
    if(!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
        InputSourcePrivate::disabledNames.removeAll(factory->properties().shortName);
    else
        InputSourcePrivate::disabledNames.append(factory->properties().shortName);

    InputSourcePrivate::disabledNames.removeDuplicates();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Transports/disabled_plugins", InputSourcePrivate::disabledNames);
}

bool InputSource::isEnabled(const InputSourceFactory *factory)
{
    InputSourcePrivate::loadPlugins();
    return !InputSourcePrivate::disabledNames.contains(factory->properties().shortName);
}
