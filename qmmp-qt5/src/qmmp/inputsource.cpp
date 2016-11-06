/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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

#include <QFile>
#include <QDir>
#include "qmmpplugincache_p.h"
#include "qmmp.h"
#include "fileinputsource_p.h"
#include "emptyinputsource_p.h"
#include "inputsource.h"

InputSource::InputSource(const QString &source, QObject *parent) : QObject(parent)
{
    m_url = source;
    m_offset = -1;
    m_hasMetaData = false;
    m_hasStreamInfo = false;
}

bool InputSource::isWaiting()
{
    return false;
}

QString InputSource::contentType() const
{
    return QString();
}

const QString InputSource::url() const
{
    return m_url;
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
QList<QmmpPluginCache*> *InputSource::m_cache = 0;

InputSource *InputSource::create(const QString &url, QObject *parent)
{
    loadPlugins();
    InputSourceFactory *factory = 0;
    if(!url.contains("://")) //local file path doesn't contain "://"
    {
        qDebug("InputSource: using file transport");
        return new FileInputSource(url, parent);
    }
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;

        factory = item->inputSourceFactory();

        if(factory && factory->properties().protocols.contains(url.section("://", 0, 0)))
            break;
        else
            factory = 0;
    }
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
    foreach (QmmpPluginCache *item, *m_cache)
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
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->decoderFactory())
            list.append(item->inputSourceFactory());
    }
    return list;
}

QString InputSource::file(InputSourceFactory *factory)
{
    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

QStringList InputSource::protocols()
{
    loadPlugins();
    QStringList protocolsList;

    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_disabledNames.contains(item->shortName()))
            continue;
        if(item->inputSourceFactory())
            protocolsList << item->inputSourceFactory()->properties().protocols;
    }
    protocolsList.removeDuplicates();
    return protocolsList;
}

void InputSource::setEnabled(InputSourceFactory *factory, bool enable)
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
    settings.setValue("Transports/disabled_plugins", m_disabledNames);
}

bool InputSource::isEnabled(InputSourceFactory *factory)
{
    loadPlugins();
    return !m_disabledNames.contains(factory->properties().shortName);
}

void InputSource::loadPlugins()
{
    if (m_cache)
        return;

    m_cache = new QList<QmmpPluginCache*>;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QDir pluginsDir (Qmmp::pluginsPath());
#ifndef Q_OS_ANDROID
    pluginsDir.cd("Transports");
#endif
    QStringList filters;
    filters << "*.dll" << "*.so";
    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files))
    {
#ifdef Q_OS_ANDROID
        if(!fileName.contains("_transports_"))
        {
            continue;
        }
#endif
        QmmpPluginCache *item = new QmmpPluginCache(pluginsDir.absoluteFilePath(fileName), &settings);
        if(item->hasError())
        {
            delete item;
            continue;
        }
        m_cache->append(item);
    }
    m_disabledNames = settings.value("Transports/disabled_plugins").toStringList();
}
