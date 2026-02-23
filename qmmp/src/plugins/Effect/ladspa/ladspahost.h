/***************************************************************************
 *   Copyright (C) 2002,2003 Nick Lamb <njl195@zepler.org.uk>              *
 *   Copyright (C) 2005 Giacomo Lozito <city_hunter@users.sf.net>          *
 *   Copyright (C) 2009-2026 by Ilya Kotov <forkotov02@ya.ru>              *
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

#ifndef LADSPAHOST_H
#define LADSPAHOST_H

#include <QList>
#include <QObject>
#include "ladspa.h"

class QWidget;

#define MAX_SAMPLES 8192
#define MAX_KNOBS 64

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class LADSPAPlugin
{
public:
    QString name;
    long id;
    long unique_id;
    const LADSPA_Descriptor *desc;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class LADSPAControl
{
public:
    enum Type
    {
        Button = 0,
        Slider,
        Label
    };

    double min;
    double max;
    double step;
    LADSPA_Data value;
    int type;
    int port;
    QString name;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class LADSPAEffect
{
public:
    LADSPAPlugin *plugin;
    QList<int> in_ports, out_ports;
    QList<LADSPA_Handle> handles;
    QList<LADSPAControl*> controls;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class LADSPAHost : public QObject
{
    Q_OBJECT
public:
    explicit LADSPAHost(QObject *parent);
    virtual ~LADSPAHost();

    int applyEffect(float *data, size_t samples);
    void configure(quint32 freq, int chan);
    const QList<LADSPAPlugin*> &plugins() const;
    const QList<LADSPAEffect*> &effects() const;
    void load(LADSPAPlugin *plugin);
    void unload(LADSPAEffect *effect);

    static LADSPAHost* instance();

private:
    void loadModules();
    void findModules(const QString &path);
    void unloadModules();
    LADSPAEffect *createEffect(LADSPAPlugin *plugin);
    LADSPAControl *createControl(const LADSPA_Descriptor *desc, unsigned long port);
    void activateEffect(LADSPAEffect *e);
    void deactivateEffect(LADSPAEffect *e);

    QList<LADSPAPlugin*> m_plugins;
    QList<LADSPAEffect*> m_effects;

    int m_chan = 2;
    quint32 m_freq = 44100;
    QList<void*> m_modules;
    LADSPA_Data m_buf[9][MAX_SAMPLES];
    static LADSPAHost *m_instance;

};

#endif
