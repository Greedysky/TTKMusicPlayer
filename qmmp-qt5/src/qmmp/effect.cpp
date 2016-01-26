/***************************************************************************
 *   Copyright (C) 2007-2014 by Ilya Kotov                                 *
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

#include <algorithm>
#include <QStringList>
#include <QDir>
#include "qmmpaudioengine_p.h"
#include "qmmp.h"
#include "qmmpplugincache_p.h"
#include "effectfactory.h"
#include "effect.h"

Effect::Effect()
{
    m_freq = 0;
    m_format = Qmmp::PCM_UNKNOWM;
    m_channels = 0;
    m_factory = 0;
}

Effect::~Effect()
{}

void Effect::configure(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    m_freq = freq;
    m_chan_map = map;
    m_channels = map.count();
    m_format = format;
}

quint32 Effect::sampleRate() const
{
    return m_freq;
}

int Effect::channels() const
{
    return m_channels;
}

const ChannelMap Effect::channelMap() const
{
    return m_chan_map;
}

Qmmp::AudioFormat Effect::format() const
{
    return m_format;
}

const AudioParameters Effect::audioParameters() const
{
    return AudioParameters(m_freq, m_chan_map, m_format);
}

EffectFactory* Effect::factory() const
{
    return m_factory;
}

bool _effectCacheCompareFunc(QmmpPluginCache *e1, QmmpPluginCache *e2)
{
    return e1->priority() > e2->priority();
}

//static members

QList<QmmpPluginCache*> *Effect::m_cache = 0;
QStringList Effect::m_enabledNames;

void Effect::loadPlugins()
{
    if(m_cache)
        return;

    m_cache = new QList<QmmpPluginCache *>;
    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    QDir pluginsDir (Qmmp::pluginsPath());
    pluginsDir.cd("Effect");
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

    std::stable_sort(m_cache->begin(), m_cache->end(), _effectCacheCompareFunc);
    m_enabledNames = settings.value("Effect/enabled_plugins").toStringList();
}

Effect* Effect::create(EffectFactory *factory)
{
    loadPlugins();
    Effect *effect = factory->create();
    effect->m_factory = factory;
    return effect;
}

QList<EffectFactory *> Effect::factories()
{
    loadPlugins();
    QList<EffectFactory *> list;
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(item->effectFactory())
            list.append(item->effectFactory());
    }
    return list;
}

QList<EffectFactory *> Effect::enabledFactories()
{
    loadPlugins();
    QList<EffectFactory *> list;
    foreach (QmmpPluginCache *item, *m_cache)
    {
        if(m_enabledNames.contains(item->shortName()) && item->effectFactory())
            list.append(item->effectFactory());
    }
    return list;
}

QString Effect::file(EffectFactory *factory)
{
    loadPlugins();
    foreach(QmmpPluginCache *item, *m_cache)
    {
        if(item->shortName() == factory->properties().shortName)
            return item->file();
    }
    return QString();
}

void Effect::setEnabled(EffectFactory* factory, bool enable)
{
    loadPlugins();
    if (!factories().contains(factory))
        return;

    if(enable == isEnabled(factory))
        return;

    if(enable)
    {
        if(QmmpAudioEngine::instance())
            QmmpAudioEngine::instance()->addEffect(factory);
        m_enabledNames.append(factory->properties().shortName);
    }
    else
    {
        m_enabledNames.removeAll(factory->properties().shortName);
        if(QmmpAudioEngine::instance())
            QmmpAudioEngine::instance()->removeEffect(factory);
    }

    m_enabledNames.removeDuplicates();

    QSettings settings (Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("Effect/enabled_plugins", m_enabledNames);
}

bool Effect::isEnabled(EffectFactory* factory)
{
    loadPlugins();
    return m_enabledNames.contains(factory->properties().shortName);
}
