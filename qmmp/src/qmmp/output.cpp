#include "qmmpplugincache_p.h"
#include "output.h"

class OutputPrivate
{
public:
    static void loadPlugins()
    {
        if(cache)
            return;

        cache = new QList<QmmpPluginCache *>;
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        for(const QString &filePath : Qmmp::findPlugins("Output"))
        {
            QmmpPluginCache *item = new QmmpPluginCache(filePath, &settings);
            if(item->hasError())
            {
                delete item;
                continue;
            }
            cache->append(item);
        }
        QmmpPluginCache::cleanup(&settings);
    }

    quint32 frequency = 0;
    ChannelMap channelMap;
    Qmmp::AudioFormat format = Qmmp::PCM_UNKNOWN;
    int sampleSize = 0;

    static QList<QmmpPluginCache*> *cache;

};

QList<QmmpPluginCache*> *OutputPrivate::cache = nullptr;


Output::Output()
    : d(new OutputPrivate)
{

}

Output::~Output()
{

}

void Output::configure(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    d->frequency = freq;
    d->channelMap = map;
    d->format = format;
    d->sampleSize = AudioParameters::sampleSize(format);
}

AudioParameters Output::audioParameters() const
{
    return AudioParameters(d->frequency, d->channelMap, d->format);
}

quint32 Output::sampleRate() const
{
    return d->frequency;
}

int Output::channels() const
{
    return d->channelMap.count();
}

const ChannelMap &Output::channelMap() const
{
    return d->channelMap;
}

Qmmp::AudioFormat Output::format() const
{
    return d->format;
}

int Output::sampleSize() const
{
    return d->sampleSize;
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

Output *Output::create()
{
    OutputPrivate::loadPlugins();
    Output *output = nullptr;
    if(OutputPrivate::cache->isEmpty())
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
    OutputPrivate::loadPlugins();
    QList<OutputFactory *> list;
    for(QmmpPluginCache *item : qAsConst(*OutputPrivate::cache))
    {
        if(item->outputFactory())
            list.append(item->outputFactory());
    }
    return list;
}

QString Output::file(const OutputFactory *factory)
{
    OutputPrivate::loadPlugins();
    for(const QmmpPluginCache *item : qAsConst(*OutputPrivate::cache))
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

void Output::setCurrentFactory(const OutputFactory *factory)
{
    OutputPrivate::loadPlugins();
    if(file(factory).isEmpty())
        return;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Output/current_plugin", factory->properties().shortName);
}

OutputFactory *Output::currentFactory()
{
    OutputPrivate::loadPlugins();

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
    for(QmmpPluginCache *item : qAsConst(*OutputPrivate::cache))
    {
        if(item->shortName() == name && item->outputFactory())
            return item->outputFactory();
    }
    if(!OutputPrivate::cache->isEmpty())
        return OutputPrivate::cache->at(0)->outputFactory();
    return nullptr;
}
