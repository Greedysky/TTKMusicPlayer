/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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

#ifndef QMMPPLUGINCACHE_P_H
#define QMMPPLUGINCACHE_P_H

#include <QString>
#include <QObject>
#include <QSettings>
#include "qmmp_export.h"

class DecoderFactory;
class OutputFactory;
class EngineFactory;
class EffectFactory;
class InputSourceFactory;

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT QmmpPluginCache
{
public:
    QmmpPluginCache(const QString &file, QSettings *settings);

    const QString shortName() const;
    const QString file() const;
    int priority() const;
    bool hasError() const;

    DecoderFactory *decoderFactory();
    OutputFactory *outputFactory();
    EngineFactory *engineFactory();
    EffectFactory *effectFactory();
    InputSourceFactory *inputSourceFactory();

    static void cleanup(QSettings *settings);

private:
    QObject *instance();
    QString m_path;
    QString m_shortName;
    bool m_error;
    QObject *m_instance;
    DecoderFactory *m_decoderFactory;
    OutputFactory *m_outputFactory;
    EngineFactory *m_engineFactory;
    EffectFactory *m_effectFactory;
    InputSourceFactory *m_inputSourceFactory;
    int m_priority;
};

#endif // QMMPPLUGINCACHE_P_H
