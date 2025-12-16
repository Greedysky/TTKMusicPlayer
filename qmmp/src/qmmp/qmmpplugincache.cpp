#include <QDateTime>
#include <QFileInfo>
#include <QPluginLoader>
#include <QApplication>
#include "decoderfactory.h"
#include "outputfactory.h"
#include "enginefactory.h"
#include "effectfactory.h"
#include "inputsourcefactory.h"
#include "qmmpplugincache_p.h"

QmmpPluginCache::QmmpPluginCache(const QString &file, QSettings *settings)
{
    bool update = false;
    const QFileInfo fin(file);
    m_path = fin.canonicalFilePath();

    settings->beginGroup("PluginCache");
    QString key = m_path;
#ifndef Q_OS_WIN
    key.remove(0,1);
#endif
    if(settings->allKeys().contains(key))
    {
        const QStringList &values = settings->value(m_path).toStringList();
        if(values.count() != 6)
            update = true;
        else
        {
            m_shortName = values.at(0);
            m_priority = values.at(1).toInt();
            m_protocols = values.at(2).split(";", QtSkipEmptyParts);
            m_filters = values.at(3).split(";", QtSkipEmptyParts);
            m_contentTypes = values.at(4).split(";", QtSkipEmptyParts);
            update = (fin.lastModified().toString(Qt::ISODate) != values.at(5));
        }
    }
    else
        update = true;

    if(update)
    {
        if(DecoderFactory *factory = decoderFactory())
        {
            m_shortName = factory->properties().shortName;
            m_priority = factory->properties().priority;
            m_protocols = factory->properties().protocols;
            m_filters = factory->properties().filters;
            m_contentTypes = factory->properties().contentTypes;
        }
        else if(OutputFactory *factory = outputFactory())
        {
            m_shortName = factory->properties().shortName;
            m_priority = 0;
        }
        else if(EngineFactory *factory = engineFactory())
        {
            m_shortName = factory->properties().shortName;
            m_priority = 0;
            m_protocols = factory->properties().protocols;
            m_filters = factory->properties().filters;
            m_contentTypes = factory->properties().contentTypes;
        }
        else if(EffectFactory *factory = effectFactory())
        {
            m_shortName = factory->properties().shortName;
            m_priority = factory->properties().priority;
        }
        else if(InputSourceFactory *factory = inputSourceFactory())
        {
            m_shortName = factory->properties().shortName;
            m_priority = 0;
            m_protocols = factory->properties().protocols;
        }
        else
        {
            qWarning("QmmpPluginCache: unknown plugin type: %s", qPrintable(m_path));
            m_error = true;
        }

        if(!m_error)
        {
            QStringList values;
            values << m_shortName;
            values << QString::number(m_priority);
            values << m_protocols.join(";");
            values << m_filters.join(";");
            values << m_contentTypes.join(";");
            values << fin.lastModified().toString(Qt::ISODate);
            settings->setValue(m_path, values);
            qDebug("QmmpPluginCache: added cache item \"%s=%s\"", qPrintable(fin.fileName()), qPrintable(values.join(",")));
        }
    }
    settings->endGroup();
}

QString QmmpPluginCache::shortName() const
{
    return m_shortName;
}

QString QmmpPluginCache::file() const
{
    return m_path;
}

QStringList QmmpPluginCache::filters() const
{
    if(m_decoderFactory)
        return m_decoderFactory->properties().filters;

    if(m_engineFactory)
        return m_engineFactory->properties().filters;

    return m_filters;
}

const QStringList &QmmpPluginCache::contentTypes() const
{
    return m_contentTypes;
}

const QStringList &QmmpPluginCache::protocols() const
{
    return m_protocols;
}

int QmmpPluginCache::priority() const
{
    return m_priority;
}

DecoderFactory *QmmpPluginCache::decoderFactory()
{
    if(!m_decoderFactory)
    {
        m_decoderFactory = qobject_cast<DecoderFactory *> (instance());
    }
    return m_decoderFactory;
}

OutputFactory *QmmpPluginCache::outputFactory()
{
    if(!m_outputFactory)
    {
        m_outputFactory = qobject_cast<OutputFactory *> (instance());
    }
    return m_outputFactory;
}

EngineFactory *QmmpPluginCache::engineFactory()
{
    if(!m_engineFactory)
    {
        m_engineFactory = qobject_cast<EngineFactory *> (instance());
    }
    return m_engineFactory;
}

EffectFactory *QmmpPluginCache::effectFactory()
{
    if(!m_effectFactory)
    {
        m_effectFactory = qobject_cast<EffectFactory *> (instance());
    }
    return m_effectFactory;
}

InputSourceFactory *QmmpPluginCache::inputSourceFactory()
{
    if(!m_inputSourceFactory)
    {
        m_inputSourceFactory = qobject_cast<InputSourceFactory *> (instance());
    }
    return m_inputSourceFactory;
}

void QmmpPluginCache::update(QSettings *settings)
{
    //save changed filters list only
    if(m_filters != filters())
    {
        m_filters = filters();

        settings->beginGroup("PluginCache");
        QStringList values = settings->value(m_path).toStringList();
        if(values.count() == 6)
        {
            values[3] = m_filters.join(";");
            settings->setValue(m_path, values);
        }
        settings->endGroup();
    }
}

bool QmmpPluginCache::hasError() const
{
    return m_error;
}

QObject *QmmpPluginCache::instance()
{
    if(m_error)
        return nullptr;
    if(m_instance)
        return m_instance;
    QPluginLoader loader(m_path);
    m_instance = loader.instance();
    if(loader.isLoaded())
        qDebug("QmmpPluginCache: loaded plugin %s", qPrintable(QFileInfo(m_path).fileName()));
    else
    {
        m_error = true;
        m_filters.clear();
        m_contentTypes.clear();
        m_protocols.clear();
        qWarning("QmmpPluginCache: error: %s", qPrintable(loader.errorString()));
    }
    return m_instance;
}

void QmmpPluginCache::cleanup(QSettings *settings)
{
    settings->beginGroup("PluginCache");

    for(const QString &key : settings->allKeys())
    {
#ifdef Q_OS_WIN
        if(!QFile::exists(key))
#else
        if(!QFile::exists("/" + key))
#endif
        {
            settings->remove(key);
            qDebug("QmmpPluginCache: removed key %s", qPrintable(key));
        }
    }
    settings->endGroup();
}
