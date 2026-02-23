/***************************************************************************
 *   Copyright (C) 2007-2026 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#ifndef EFFECT_H
#define EFFECT_H

#include <QList>
#include <QStringList>
#include "audioparameters.h"
#include "channelmap.h"
#include "buffer.h"
#include "effectfactory.h"

class QmmpPluginCache;

/*! @brief The Effect class provides the base interface class of audio effects.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Effect
{
public:
    /*!
     * Object contsructor.
     */
    Effect() = default;
    /*!
     * Destructor.
     */
    virtual ~Effect() = default;

    /*!
     * Adds audio effect to the buffer \b b.
     * Subclass should implement this function.
     */
    virtual void applyEffect(Buffer *b) = 0;
    /*!
     * Prepares object for usage.
     * Subclasses that reimplement this function must call the base implementation.
     * @param srate Sample rate.
     * @param map Map of channels.
     */
    virtual void configure(quint32 srate, ChannelMap map);
    /*!
     * Returns samplerate.
     */
    quint32 sampleRate() const;
    /*!
     * Returns channels number.
     */
    int channels() const;
    /*!
     * Returns map of channels
     */
    const ChannelMap &channelMap() const;
    /*!
     * Returns audio parameters for output data.
     */
    AudioParameters audioParameters() const;
    /*!
     * Returns assigned factory object.
     */
    EffectFactory* factory() const;
    /*!
     * Creates effect object from \b factory. Returns effect objects if factory is enabled,
     * otherwise returns \b 0.
     */
    static Effect* create(EffectFactory *factory);
    /*!
     * Returns a list of effect factories.
     */
    static QList<EffectFactory*> factories();
    /*!
     * Returns a list of enabled effect factories.
     */
    static QList<EffectFactory*> enabledFactories();
    /*!
     * Returns plugin file path.
     * @param factory Effect plugin factory.
     */
    static QString file(const EffectFactory *factory);
    /*!
     * Sets whether the effect plugin is enabled.
     * @param factory Effect plugin factory.
     * @param enable Plugin enable state (\b true - enable, \b false - disable)
     */
    static void setEnabled(EffectFactory *factory, bool enable = true);
    /*!
     * Returns \b true if input plugin is enabled, otherwise returns \b false
     * @param factory Effect plugin factory.
     */
    static bool isEnabled(const EffectFactory* factory);
    /*!
     * Finds effect factory object by short name \b shortName. Returns effect factory pointer.
     * If no match is found, 0 is returned. The search is case sensitive.
     */
    static EffectFactory *findFactory(const QString &shortName);

private:
    EffectFactory *m_factory = nullptr;
    quint32 m_freq = 0;
    int m_channels = 0;
    ChannelMap m_chan_map;

    static void loadPlugins();
    static QList<QmmpPluginCache*> *m_cache;
    static QStringList m_enabledNames;

};

#endif
