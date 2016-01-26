/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
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

#include <QSettings>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include "enginefactory.h"
#include "qmmpaudioengine_p.h"
#include "qmmpplugincache_p.h"
#include "statehandler.h"
#include "qmmp.h"
#include "abstractengine.h"

AbstractEngine::AbstractEngine(QObject *parent) : QThread(parent)
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
QList<QmmpPluginCache*> *AbstractEngine::m_cache = 0;

void AbstractEngine::loadPlugins()
{
    if (m_cache)
        return;

    m_cache = new QList<QmmpPluginCache*>;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QDir pluginsDir (Qmmp::pluginsPath());
    pluginsDir.cd("Engines");
    QStringList filters;
    filters << "*.dll" << "*.so";
    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files))
    {
        QmmpPluginCache *item = new QmmpPluginCache(pluginsDir.absoluteFilePath(fileName), &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    m_disabledNames = settings.value("Engine/disabled_plugins").toStringList();
}

AbstractEngine *AbstractEngine::create(InputSource *s, QObject *parent)
{
    AbstractEngine *engine = new QmmpAudioEngine(parent); //internal engine
    if(!engine->enqueue(s))
    {
        engine->deleteLater();
        engine = 0;
    }
    else
        return engine;


    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        EngineFactory *fact = item->engineFactory();
        if(!fact)
            continue;
        engine = fact->create(parent); //engine plugin
        if(!engine->enqueue(s))
        {
            engine->deleteLater();
            engine = 0;
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
    foreach (QmmpPluginCache *item, *m_cache)
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
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->engineFactory())
            list.append(item->engineFactory());
    }
    return list;
}

EngineFactory *AbstractEngine::findByPath(const QString& source)
{
    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        EngineFactory *fact = item->engineFactory();
        if (fact && fact->supports(source))
            return fact;
    }
    return 0;
}

void AbstractEngine::setEnabled(EngineFactory* factory, bool enable)
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
    settings.setValue("Engine/disabled_plugins", m_disabledNames);
}

bool AbstractEngine::isEnabled(EngineFactory* factory)
{
    loadPlugins();
    return !m_disabledNames.contains(factory->properties().shortName);
}

QString AbstractEngine::file(EngineFactory *factory)
{
    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList AbstractEngine::protocols()
{
    loadPlugins();
    QStringList protocolsList;

    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->engineFactory())
            protocolsList << item->engineFactory()->properties().protocols;
    }
    protocolsList.removeDuplicates();
    return protocolsList;
}
