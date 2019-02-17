// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//
#include <QStringList>
#include <QSettings>
#include <QBuffer>
#include <QDir>
#include <math.h>
#include <algorithm>
#include "qmmpplugincache_p.h"
#include "output.h"
#include "decoderfactory.h"

extern "C"
{
#include "equ/iir.h"
}
#include "decoder.h"

Decoder::Decoder(QIODevice *input) : m_input(input)
{
    m_hasMetaData = false;
}

Decoder::~Decoder()
{}

void Decoder::setReplayGainInfo(const QMap<Qmmp::ReplayGainKey, double> &rg)
{
    m_rg = rg;
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
    m_parameters = p;
    setProperty(Qmmp::SAMPLERATE, m_parameters.sampleRate());
    setProperty(Qmmp::CHANNELS, m_parameters.channels());
    setProperty(Qmmp::BITS_PER_SAMPLE, m_parameters.validBitsPerSample());
}

void Decoder::next()
{}

const QString Decoder::nextURL() const
{
    return QString();
}

AudioParameters Decoder::audioParameters() const
{
    return m_parameters;
}

QMap<Qmmp::ReplayGainKey, double> Decoder::replayGainInfo() const
{
    return m_rg;
}

void Decoder::addMetaData(const QMap<Qmmp::MetaData, QString> &metaData)
{
    m_metaData = metaData;
    m_hasMetaData = true;
}

QIODevice *Decoder::input()
{
    return m_input;
}

bool Decoder::hasMetaData() const
{
    return m_hasMetaData;
}

QMap<Qmmp::MetaData, QString> Decoder::takeMetaData()
{
    m_hasMetaData = false;
    return m_metaData;
}

void Decoder::setProperty(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        m_properties.remove(key);
    else
        m_properties[key] = strValue;
}

void Decoder::setProperties(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    foreach(Qmmp::TrackProperty key, properties.keys())
    {
        setProperty(key, properties.value(key));
    }
}

const QMap<Qmmp::TrackProperty, QString> &Decoder::properties() const
{
    return m_properties;
}

// static methods
QStringList Decoder::m_disabledNames;
QList<QmmpPluginCache*> *Decoder::m_cache = nullptr;

//sort cache items by priority
static bool _pluginCacheLessComparator(QmmpPluginCache* f1, QmmpPluginCache* f2)
{
    return f1->priority() < f2->priority();
}

void Decoder::loadPlugins()
{
    if (m_cache)
        return;

    m_cache = new QList<QmmpPluginCache*>;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    foreach (QString filePath, Qmmp::findPlugins("Input"))
    {
        QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    m_disabledNames = settings.value("Decoder/disabled_plugins").toStringList();
    std::stable_sort(m_cache->begin(), m_cache->end(), _pluginCacheLessComparator);
    QmmpPluginCache::cleanup(&settings);
}

QString Decoder::file(DecoderFactory *factory)
{
    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList Decoder::protocols()
{
    loadPlugins();
    QStringList protocolsList;

    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->decoderFactory())
            protocolsList << item->decoderFactory()->properties().protocols;
    }
    protocolsList.removeDuplicates();
    return protocolsList;
}

DecoderFactory *Decoder::findByFilePath(const QString &path, bool useContent)
{
    loadPlugins();
    DecoderFactory *fact = nullptr;
    //detect by content if enabled
    if(useContent)
    {
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly))
        {
            qWarning("Decoder: file open error: %s", qPrintable(file.errorString()));
            return nullptr;
        }

        foreach(QmmpPluginCache *item, *m_cache)
        {
            if(m_disabledNames.contains(item->shortName()))
                continue;

            if(!(fact = item->decoderFactory()))
                continue;

            if(fact->properties().noInput && !fact->properties().protocols.contains("file"))
                continue;

            if (fact->canDecode(&file))
                return fact;
        }
        fact = nullptr;
    }

    QList<DecoderFactory*> filtered = findByFileExtension(path);

    if(filtered.isEmpty())
        return nullptr;

    if(filtered.size() == 1)
        return filtered.at(0);

    //more than one factories with same filters
    //try to determine by content
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("Decoder: file open error: %s", qPrintable(file.errorString()));
        return nullptr;
    }

    foreach (fact, filtered)
    {
        if(fact->canDecode(&file))
            return fact;
    }

    if(!filtered.isEmpty() && !useContent) //fallback
        return filtered.first();

    return nullptr;
}

DecoderFactory *Decoder::findByMime(const QString& type)
{
    if(type.isEmpty())
        return nullptr;
    loadPlugins();
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        DecoderFactory *fact = item->decoderFactory();
        if(fact && !fact->properties().noInput && fact->properties().contentTypes.contains(type))
            return fact;
    }
    return nullptr;
}

DecoderFactory *Decoder::findByContent(QIODevice *input)
{
    loadPlugins();
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        DecoderFactory *fact = item->decoderFactory();
        if(fact && !fact->properties().noInput && fact->canDecode(input))
            return fact;
    }
    return nullptr;
}

DecoderFactory *Decoder::findByProtocol(const QString &p)
{
    loadPlugins();
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        if (item->decoderFactory() && item->decoderFactory()->properties().protocols.contains(p))
            return item->decoderFactory();
    }
    return nullptr;
}

QList<DecoderFactory *> Decoder::findByFileExtension(const QString &path)
{
    QList<DecoderFactory*> filtered;
    DecoderFactory *fact = nullptr;
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        if(!(fact = item->decoderFactory()))
            continue;

        foreach(QString filter, fact->properties().filters)
        {
            QRegExp regexp(filter, Qt::CaseInsensitive, QRegExp::Wildcard);
            if (regexp.exactMatch(path))
            {
                filtered.append(fact);
                break;
            }
        }
    }

    return filtered;
}

void Decoder::setEnabled(DecoderFactory* factory, bool enable)
{
    loadPlugins();
    if (!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
        m_disabledNames.removeAll(factory->properties().shortName);
    else
        m_disabledNames.append(factory->properties().shortName);

    m_disabledNames.removeDuplicates();
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Decoder/disabled_plugins", m_disabledNames);
}

bool Decoder::isEnabled(DecoderFactory* factory)
{
    loadPlugins();
    return !m_disabledNames.contains(factory->properties().shortName);
}

QList<DecoderFactory *> Decoder::factories()
{
    loadPlugins();
    QList<DecoderFactory *> list;
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(item->decoderFactory())
            list.append(item->decoderFactory());
    }
    return list;
}

QList<DecoderFactory *> Decoder::enabledFactories()
{
    loadPlugins();
    QList<DecoderFactory *> list;
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->decoderFactory())
            list.append(item->decoderFactory());
    }
    return list;
}
