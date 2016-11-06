/***************************************************************************
 *   Copyright (C) 2002-2003 Nick Lamb <njl195@zepler.org.uk>              *
 *   Copyright (C) 2005 Giacomo Lozito <city_hunter@users.sf.net>          *
 *   Copyright (C) 2009-2015 by Ilya Kotov <forkotov02@hotmail.ru>         *
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
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <math.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <qmmp/qmmp.h>
#include <qmmp/audioparameters.h>
#include "ladspahost.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

LADSPAHost *LADSPAHost::m_instance = 0;

/* Based on xmms_ladspa */

LADSPAHost::LADSPAHost(QObject *parent) : QObject(parent)
{
    m_chan = 2;
    m_freq = 44100;
    m_instance = this;
    loadModules();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    int p = settings.value("LADSPA/plugins_number", 0).toInt();
    for(int i = 0; i < p; ++i)
    {
        QString section = QString("LADSPA_%1/").arg(i);
        settings.beginGroup(section);

        int id = settings.value("id").toInt();

        LADSPAPlugin *plugin = 0;
        foreach(LADSPAPlugin *p, plugins())
        {
            if(p->unique_id == id)
            {
                plugin = p;
                break;
            }
        }
        if(!plugin)
            continue;

        LADSPAEffect *effect = createEffect(plugin);
        foreach (LADSPAControl *c, effect->controls)
            c->value = settings.value(QString("port%1").arg(c->port), c->value).toFloat();

        m_effects.append(effect);
        settings.endGroup();
    }
}

LADSPAHost::~LADSPAHost()
{
    m_instance = 0;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    for(int i = 0; i < settings.value("LADSPA/plugins_number", 0).toInt(); ++i)
    {
        settings.remove(QString("LADSPA_%1/").arg(i));
    }
    settings.setValue("LADSPA/plugins_number", m_effects.count());
    for(int i = 0; i < m_effects.count(); ++i)
    {
        QString section = QString("LADSPA_%1/").arg(i);
        settings.beginGroup(section);

        settings.setValue("id", (quint64)m_effects[i]->plugin->desc->UniqueID);

        foreach (LADSPAControl *c, m_effects[i]->controls)
            settings.setValue(QString("port%1").arg(c->port), c->value);

        settings.endGroup();
    }
    unloadModules();
}

void LADSPAHost::configure(quint32 freq, int chan)
{
    m_chan = chan;
    m_freq = freq;

    foreach (LADSPAEffect *e, m_effects)
    {
        //deactivate effect
        deactivateEffect(e);
        //update controls for new sample rate
        for(int i = 0; i < e->controls.count(); ++i)
        {
            LADSPAControl *c = e->controls[i];
            unsigned long port = c->port;
            if(LADSPA_IS_HINT_SAMPLE_RATE(e->plugin->desc->PortRangeHints[port].HintDescriptor))
            {
                double value = c->value;
                delete c;
                e->controls[i] = createControl(e->plugin->desc, port);
                e->controls[i]->value = value; //restore value
            }
        }
        activateEffect(e);
    }
}

LADSPAHost* LADSPAHost::instance()
{
    return m_instance;
}

void LADSPAHost::loadModules()
{
    if(!m_modules.isEmpty())
        return;

    QString ladspa_path = qgetenv("LADSPA_PATH");
    QStringList directories;

    if (ladspa_path.isEmpty())
    {
        /* Fallback, look in obvious places */
        directories << "/usr/lib/ladspa";
        directories << "/usr/local/lib/ladspa";
        directories << "/usr/lib64/ladspa";
        directories << "/usr/local/lib64/ladspa";
    }
    else
        directories = ladspa_path.split(':');
    foreach(QString dir, directories)
        findModules(dir);
}

void LADSPAHost::findModules(const QString &path)
{
    QDir dir (path);
    dir.setFilter(QDir::Files | QDir::Hidden);
    dir.setSorting(QDir::Name);
    QFileInfoList files = dir.entryInfoList((QStringList() << "*.so"));

    foreach(QFileInfo file, files)
    {
        void *library = dlopen(qPrintable(file.absoluteFilePath ()), RTLD_LAZY);
        if (!library)
            continue;

        LADSPA_Descriptor_Function descriptor_fn = (LADSPA_Descriptor_Function) dlsym(library, "ladspa_descriptor");
        if (!descriptor_fn)
        {
            dlclose(library);
            continue;
        }

        m_modules.append(library);

        unsigned long k = 0;
        const LADSPA_Descriptor *descriptor;

        while((descriptor = descriptor_fn(k)) != 0)
        {
            if(LADSPA_IS_INPLACE_BROKEN(descriptor->Properties))
            {
                qWarning("LADSPAHost: plugin %s is ignored due to LADSPA_PROPERTY_INPLACE_BROKEN property", descriptor->Name);
                continue;
            }
            LADSPAPlugin *plugin = new LADSPAPlugin;
            plugin->name = descriptor->Name;
            plugin->id = k;
            plugin->unique_id = descriptor->UniqueID;
            plugin->desc = descriptor;
            m_plugins.append(plugin);
            k++;
        }
    }
}

void LADSPAHost::unloadModules()
{
    while(!m_effects.isEmpty())
    {
        LADSPAEffect *e = m_effects.takeLast();
        deactivateEffect(e);
        delete e;
    }
    qDeleteAll(m_plugins);
    m_plugins.clear();
    while (!m_modules.isEmpty())
        dlclose(m_modules.takeFirst());
}

LADSPAEffect *LADSPAHost::createEffect(LADSPAPlugin *plugin)
{
    LADSPAEffect *effect = new LADSPAEffect;
    effect->plugin = plugin;

    for(unsigned long port = 0; port < plugin->desc->PortCount; port++)
    {
        LADSPA_PortDescriptor d = plugin->desc->PortDescriptors[port];
        if (LADSPA_IS_PORT_CONTROL(d))
        {
            effect->controls << createControl(plugin->desc, port);
        }
        else if(LADSPA_IS_PORT_AUDIO(d))
        {
            if(LADSPA_IS_PORT_INPUT(d))
                effect->in_ports << port;
            if(LADSPA_IS_PORT_OUTPUT(d))
                effect->out_ports << port;
        }
    }
    return effect;
}

LADSPAControl *LADSPAHost::createControl(const LADSPA_Descriptor *desc, unsigned long port)
{
    const LADSPA_PortRangeHint hint = desc->PortRangeHints[port];
    LADSPA_Data fact, min, max, step, start;

    LADSPAControl *c = new LADSPAControl;
    c->name = QString(desc->PortNames[port]);
    c->port = port;

    if (LADSPA_IS_HINT_TOGGLED(hint.HintDescriptor))
    {
        c->type = LADSPAControl::BUTTON;
        c->min = 0;
        c->max = 0;
        c->step = 0;
        c->value = 0;
        return c;
    }

    if (LADSPA_IS_HINT_SAMPLE_RATE(hint.HintDescriptor))
        fact = m_freq;
    else
        fact = 1.0f;

    if (LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor))
        min = hint.LowerBound * fact;
    else
        min = -10000.0f;

    if (LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor))
        max = hint.UpperBound * fact;
    else
        max = 10000.0f;

    if (10000.0f <= max - min)   // infinity
        step = 5.0f;
    else if (100.0f < max - min) // 100.0 ... lots
        step = 5.0f;
    else if (10.0f < max - min)  // 10.0 ... 100.0
        step = 0.5f;
    else if (1.0f < max - min)   // 1.0 ... 10.0
        step = 0.05f;
    else  // 0.0 ... 1.0
        step = 0.005f;

    if (LADSPA_IS_HINT_INTEGER(hint.HintDescriptor))
    {
        if (step < 1.0f)
            step = 1.0f;
    }

    if (LADSPA_IS_HINT_DEFAULT_MINIMUM(hint.HintDescriptor))
        start = min;
    else if (LADSPA_IS_HINT_DEFAULT_LOW(hint.HintDescriptor))
        start = min * 0.75f + max * 0.25f;
    else if (LADSPA_IS_HINT_DEFAULT_MIDDLE(hint.HintDescriptor))
        start = min * 0.5f + max * 0.5f;
    else if (LADSPA_IS_HINT_DEFAULT_HIGH(hint.HintDescriptor))
        start = min * 0.25f + max * 0.75f;
    else if (LADSPA_IS_HINT_DEFAULT_MAXIMUM(hint.HintDescriptor))
        start = max;
    else if (LADSPA_IS_HINT_DEFAULT_0(hint.HintDescriptor))
        start = 0.0f;
    else if (LADSPA_IS_HINT_DEFAULT_1(hint.HintDescriptor))
        start = 1.0f;
    else if (LADSPA_IS_HINT_DEFAULT_100(hint.HintDescriptor))
        start = 100.0f;
    else if (LADSPA_IS_HINT_DEFAULT_440(hint.HintDescriptor))
        start = 440.0f;
    else if (LADSPA_IS_HINT_INTEGER(hint.HintDescriptor))
        start = min;
    else if (max >= 0.0f && min <= 0.0f)
        start = 0.0f;
    else
        start = min * 0.5f + max * 0.5f;

    if(LADSPA_IS_PORT_OUTPUT(desc->PortDescriptors[port]))
        c->type = LADSPAControl::LABEL;
    else
        c->type = LADSPAControl::SLIDER;
    c->min = min;
    c->max = max;
    c->step = step;
    c->value = start;
    return c;
}

void LADSPAHost::activateEffect(LADSPAEffect *e)
{
    const LADSPA_Descriptor *desc = e->plugin->desc;
    int instance_count = 1;

    if(e->out_ports.isEmpty())
    {
        qWarning("LADSPAHost: unsupported plugin: %s", desc->Name);
        return;
    }
    else if(e->in_ports.isEmpty())
    {
        if(m_chan % e->out_ports.count())
        {
            qWarning("LADSPAHost: plugin %s does not support %d channels", desc->Name, m_chan);
            return;
        }
        instance_count = m_chan / e->out_ports.count();
    }
    else if(e->in_ports.count() == e->out_ports.count())
    {
        if(m_chan % e->in_ports.count())
        {
            qWarning("LADSPAHost: plugin %s does not support %d channels", desc->Name, m_chan);
            return;
        }
        instance_count = m_chan / e->in_ports.count();
    }
    else
    {
        qWarning("LADSPAHost: unsupported plugin: %s", desc->Name);
        return;
    }

    int in_at = 0, out_at = 0;
    for(int i = 0; i < instance_count; ++i)
    {
        LADSPA_Handle handle = desc->instantiate(desc, m_freq);

        foreach (LADSPAControl *c, e->controls)
        {
            desc->connect_port(handle, c->port, &c->value);
        }
        foreach (int port, e->in_ports)
        {
            desc->connect_port(handle, port, m_buf[in_at++]);
        }
        foreach (int port, e->out_ports)
        {
            desc->connect_port(handle, port, m_buf[out_at++]);
        }

        if(desc->activate)
            desc->activate(handle);
        e->handles << handle;
    }
}

void LADSPAHost::deactivateEffect(LADSPAEffect *e)
{
    const LADSPA_Descriptor *desc = e->plugin->desc;
    foreach (LADSPA_Handle handle, e->handles)
    {
        if(desc->deactivate)
            desc->deactivate(handle);
        desc->cleanup(handle);
    }
    e->handles.clear();
}

QList <LADSPAPlugin *> LADSPAHost::plugins()
{
    return m_plugins;
}

QList <LADSPAEffect *> LADSPAHost::effects()
{
    return m_effects;
}

void LADSPAHost::load(LADSPAPlugin *plugin)
{
    LADSPAEffect *e = createEffect(plugin);
    activateEffect(e);
    m_effects.append(e);
}

void LADSPAHost::unload(LADSPAEffect *effect)
{
    m_effects.removeAll(effect);
    deactivateEffect(effect);
    delete effect;
}

int LADSPAHost::applyEffect(float *data, size_t samples)
{
    if (m_effects.isEmpty())
        return samples;

    size_t frames = samples / m_chan;

    for(size_t i = 0; i < samples; ++i)
    {
        m_buf[i % m_chan][i / m_chan] = data[i];
    }

    for(int i = 0; i < m_effects.count(); ++i)
    {
        for(int j = 0; j < m_effects[i]->handles.count(); ++j)
        {
            m_effects[i]->plugin->desc->run(m_effects[i]->handles[j], frames);
        }
    }

    for(size_t i = 0; i < samples; ++i)
    {
        data[i] = m_buf[i % m_chan][i / m_chan];
    }
    return samples;
}
