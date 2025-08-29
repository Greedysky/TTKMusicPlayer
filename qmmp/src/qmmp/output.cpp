#include <QDir>
#include "audioparameters.h"
#include "qmmpsettings.h"
#include "buffer.h"
#include "volumehandler.h"
#include "qmmpplugincache_p.h"
#include "output.h"

void Output::configure(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    m_frequency = freq;
    m_chan_map = map;
    m_format = format;
    m_sample_size = AudioParameters::sampleSize(format);
}

AudioParameters Output::audioParameters() const
{
    return AudioParameters(m_frequency, m_chan_map, m_format);
}

quint32 Output::sampleRate() const
{
    return m_frequency;
}

int Output::channels() const
{
    return m_chan_map.count();
}

const ChannelMap &Output::channelMap() const
{
    return m_chan_map;
}

Qmmp::AudioFormat Output::format() const
{
    return m_format;
}

int Output::sampleSize() const
{
    return m_sample_size;
}

void Output::suspend()
{

}

void Output::resume()
{

}

void Output::setTrackInfo(const TrackInfo &info)
{
    Q_UNUSED(info);
}

// static methods
QList<QmmpPluginCache*> *Output::m_cache = nullptr;

void Output::loadPlugins()
{
    if(m_cache)
        return;

    m_cache = new QList<QmmpPluginCache *>;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    for(const QString &filePath : Qmmp::findPlugins("Output"))
    {
        QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    QmmpPluginCache::cleanup(&settings);
}

Output *Output::create()
{
    loadPlugins();
    Output *output = nullptr;
    if(m_cache->isEmpty())
    {
        qDebug("Output: unable to find output plugins");
        return output;
    }
    OutputFactory *fact = Output::currentFactory();
    if(fact)
        output = fact->create();
    return output;
}

QList<OutputFactory *> Output::factories()
{
    loadPlugins();
    QList<OutputFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->outputFactory())
            list.append(item->outputFactory());
    }
    return list;
}

QString Output::file(const OutputFactory *factory)
{
    loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

void Output::setCurrentFactory(const OutputFactory *factory)
{
    loadPlugins();
    if(file(factory).isEmpty())
        return;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Output/current_plugin", factory->properties().shortName);
}

OutputFactory *Output::currentFactory()
{
    loadPlugins();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
#ifdef QMMP_DEFAULT_OUTPUT
    QString name = settings.value("Output/current_plugin", QMMP_DEFAULT_OUTPUT).toString();
#else
#  ifdef Q_OS_LINUX
    QString name = settings.value("Output/current_plugin", "alsa").toString();
#  elif defined Q_OS_WIN
    QString name = settings.value("Output/current_plugin", "directsound").toString();
#  elif defined Q_OS_MAC
    QString name = settings.value("Output/current_plugin", "qtmultimedia").toString();
#  else
    QString name = settings.value("Output/current_plugin", "oss4").toString();
#  endif
#endif
    for(QmmpPluginCache *item : qAsConst(*m_cache))
    {
        if(item->shortName() == name && item->outputFactory())
            return item->outputFactory();
    }
    if(!m_cache->isEmpty())
        return m_cache->at(0)->outputFactory();
    return nullptr;
}
