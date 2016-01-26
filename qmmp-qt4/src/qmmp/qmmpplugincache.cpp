/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <QStringList>
#include <QDateTime>
#include <QFileInfo>
#include <QPluginLoader>
#include <QApplication>
#include <QTranslator>
#include "decoderfactory.h"
#include "outputfactory.h"
#include "enginefactory.h"
#include "effectfactory.h"
#include "inputsourcefactory.h"
#include "qmmpplugincache_p.h"

QmmpPluginCache::QmmpPluginCache(const QString &file, QSettings *settings)
{
    m_error = false;
    m_instance = 0;
    m_decoderFactory = 0;
    m_outputFactory = 0;
    m_engineFactory = 0;
    m_effectFactory = 0;
    m_inputSourceFactory = 0;
    m_priority = 0;
    bool update = false;
    QFileInfo info(file);
    m_path = info.QFileInfo::canonicalFilePath();

    settings->beginGroup("PluginCache");
    QString key = m_path;
#ifndef Q_OS_WIN
    key.remove(0,1);
#endif
    if(settings->allKeys().contains(key))

    {
        QStringList values = settings->value(m_path).toStringList();
        if(values.count() != 3)
            update = true;
        else
        {
            m_shortName = values.at(0);
            m_priority = values.at(1).toInt();
            update = (info.lastModified().toString(Qt::ISODate) != values.at(2));
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
        }
        else
        {
            qWarning("QmmpPluginCache: unknown plugin type: %s", qPrintable(m_path));
            m_error = true;
        }

        if (!m_error)
        {
            QStringList values;
            values << m_shortName;
            values << QString::number(m_priority);
            values << info.lastModified().toString(Qt::ISODate);
            settings->setValue(m_path, values);
            qDebug("QmmpPluginCache: added cache item \"%s=%s\"",
                   qPrintable(info.fileName()), qPrintable(values.join(",")));
        }
    }
    settings->endGroup();
}

const QString QmmpPluginCache::shortName() const
{
    return m_shortName;
}

const QString QmmpPluginCache::file() const
{
    return m_path;
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

bool QmmpPluginCache::hasError() const
{
    return m_error;
}

QObject *QmmpPluginCache::instance()
{
    if(m_error)
        return 0;
    if(m_instance)
        return m_instance;
    QPluginLoader loader(m_path);
    m_instance = loader.instance();
    if (loader.isLoaded())
        qDebug("QmmpPluginCache: loaded plugin %s", qPrintable(QFileInfo(m_path).fileName()));
    else
    {
        m_error = true;
        qWarning("QmmpPluginCache: error: %s", qPrintable(loader.errorString ()));
    }
    return m_instance;
}

void QmmpPluginCache::cleanup(QSettings *settings)
{
    settings->beginGroup("PluginCache");

    foreach (QString key, settings->allKeys())
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
