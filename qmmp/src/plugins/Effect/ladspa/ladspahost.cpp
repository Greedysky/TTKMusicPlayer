#include "ladspahost.h"

#include <QDir>
#include <QSettings>
#include <dlfcn.h>
#include <qmmp/audioparameters.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

LADSPAHost *LADSPAHost::m_instance = nullptr;

/* Based on xmms_ladspa */

LADSPAHost::LADSPAHost(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
    loadModules();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    int pluginNumber = settings.value("LADSPA/plugin_number", 0).toInt();
    for(int i = 0; i < pluginNumber; ++i)
    {
        QString section = QString("LADSPA_%1/").arg(i);
        settings.beginGroup(section);

        const int id = settings.value("id").toInt();
        auto it = std::find_if(m_plugins.constBegin(), m_plugins.constEnd(), [id](LADSPAPlugin *p){ return p->unique_id == id; });
        if(it == m_plugins.constEnd())
            continue;

        LADSPAPlugin *plugin = *it;
        LADSPAEffect *effect = createEffect(plugin);
        for(LADSPAControl *c : qAsConst(effect->controls))
            c->value = settings.value(QString("port%1").arg(c->port), c->value).toFloat();

        m_effects.append(effect);
        settings.endGroup();
    }
}

LADSPAHost::~LADSPAHost()
{
    m_instance = nullptr;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    for(int i = 0; i < settings.value("LADSPA/plugin_number", 0).toInt(); ++i)
    {
        settings.remove(QString("LADSPA_%1/").arg(i));
    }
    settings.setValue("LADSPA/plugin_number", m_effects.count());
    for(int i = 0; i < m_effects.count(); ++i)
    {
        QString section = QString("LADSPA_%1/").arg(i);
        settings.beginGroup(section);

        settings.setValue("id", (quint64)m_effects[i]->plugin->desc->UniqueID);

        for(const LADSPAControl *c : qAsConst(m_effects[i]->controls))
            settings.setValue(QString("port%1").arg(c->port), c->value);

        settings.endGroup();
    }
    unloadModules();
}

void LADSPAHost::configure(quint32 freq, int chan)
{
    m_chan = chan;
    m_freq = freq;

    for(LADSPAEffect *e : qAsConst(m_effects))
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

    const QString &ladspa_path = qgetenv("LADSPA_PATH");
    QStringList directories;

    if(ladspa_path.isEmpty())
    {
        /* Fallback, look in obvious places */
        directories << "/usr/lib/ladspa";
        directories << "/usr/local/lib/ladspa";
        directories << "/usr/lib64/ladspa";
        directories << "/usr/local/lib64/ladspa";
    }
    else
        directories = ladspa_path.split(':');
    for(const QString &dir : qAsConst(directories))
        findModules(dir);
}

void LADSPAHost::findModules(const QString &path)
{
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden);
    dir.setSorting(QDir::Name);
#ifdef Q_OS_LINUX
    const QFileInfoList &files = dir.entryInfoList({"*.so"});
#else
    const QFileInfoList &files = dir.entryInfoList({"*.dylib"});
#endif
    for(const QFileInfo &fin : qAsConst(files))
    {
        void *library = dlopen(qPrintable(fin.absoluteFilePath()), RTLD_LAZY);
        if(!library)
            continue;

        LADSPA_Descriptor_Function descriptor_fn = (LADSPA_Descriptor_Function) dlsym(library, "ladspa_descriptor");
        if(!descriptor_fn)
        {
            dlclose(library);
            continue;
        }

        m_modules.append(library);

        unsigned long k = 0;
        const LADSPA_Descriptor *descriptor;

        while((descriptor = descriptor_fn(k)) != nullptr)
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
    while(!m_modules.isEmpty())
        dlclose(m_modules.takeFirst());
}

LADSPAEffect *LADSPAHost::createEffect(LADSPAPlugin *plugin)
{
    LADSPAEffect *effect = new LADSPAEffect;
    effect->plugin = plugin;

    for(unsigned long port = 0; port < plugin->desc->PortCount; ++port)
    {
        LADSPA_PortDescriptor d = plugin->desc->PortDescriptors[port];
        if(LADSPA_IS_PORT_CONTROL(d))
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

    if(LADSPA_IS_HINT_TOGGLED(hint.HintDescriptor))
    {
        c->type = LADSPAControl::Button;
        c->min = 0;
        c->max = 0;
        c->step = 0;
        c->value = 0;
        return c;
    }

    if(LADSPA_IS_HINT_SAMPLE_RATE(hint.HintDescriptor))
        fact = m_freq;
    else
        fact = 1.0f;

    if(LADSPA_IS_HINT_BOUNDED_BELOW(hint.HintDescriptor))
        min = hint.LowerBound * fact;
    else
        min = -10000.0f;

    if(LADSPA_IS_HINT_BOUNDED_ABOVE(hint.HintDescriptor))
        max = hint.UpperBound * fact;
    else
        max = 10000.0f;

    if(10000.0f <= max - min)   // infinity
        step = 5.0f;
    else if(100.0f < max - min) // 100.0 ... lots
        step = 5.0f;
    else if(10.0f < max - min)  // 10.0 ... 100.0
        step = 0.5f;
    else if(1.0f < max - min)   // 1.0 ... 10.0
        step = 0.05f;
    else  // 0.0 ... 1.0
        step = 0.005f;

    if(LADSPA_IS_HINT_INTEGER(hint.HintDescriptor))
    {
        if(step < 1.0f)
            step = 1.0f;
    }

    if(LADSPA_IS_HINT_DEFAULT_MINIMUM(hint.HintDescriptor))
        start = min;
    else if(LADSPA_IS_HINT_DEFAULT_LOW(hint.HintDescriptor))
        start = min * 0.75f + max * 0.25f;
    else if(LADSPA_IS_HINT_DEFAULT_MIDDLE(hint.HintDescriptor))
        start = min * 0.5f + max * 0.5f;
    else if(LADSPA_IS_HINT_DEFAULT_HIGH(hint.HintDescriptor))
        start = min * 0.25f + max * 0.75f;
    else if(LADSPA_IS_HINT_DEFAULT_MAXIMUM(hint.HintDescriptor))
        start = max;
    else if(LADSPA_IS_HINT_DEFAULT_0(hint.HintDescriptor))
        start = 0.0f;
    else if(LADSPA_IS_HINT_DEFAULT_1(hint.HintDescriptor))
        start = 1.0f;
    else if(LADSPA_IS_HINT_DEFAULT_100(hint.HintDescriptor))
        start = 100.0f;
    else if(LADSPA_IS_HINT_DEFAULT_440(hint.HintDescriptor))
        start = 440.0f;
    else if(LADSPA_IS_HINT_INTEGER(hint.HintDescriptor))
        start = min;
    else if(max >= 0.0f && min <= 0.0f)
        start = 0.0f;
    else
        start = min * 0.5f + max * 0.5f;

    if(LADSPA_IS_PORT_OUTPUT(desc->PortDescriptors[port]))
        c->type = LADSPAControl::Label;
    else
        c->type = LADSPAControl::Slider;
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

    if(e->in_ports.isEmpty())
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
        if(handle == nullptr)
        {
            qWarning("LADSPAHost: failed to instantiate plugin: %s", desc->Name);
            continue;
        }

        for(LADSPAControl *c : qAsConst(e->controls))
        {
            desc->connect_port(handle, c->port, &c->value);
        }
        for(int port : qAsConst(e->in_ports))
        {
            desc->connect_port(handle, port, m_buf[in_at++]);
        }
        for(int port : qAsConst(e->out_ports))
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
    for(const LADSPA_Handle &handle : qAsConst(e->handles))
    {
        if(desc->deactivate)
            desc->deactivate(handle);
        desc->cleanup(handle);
    }
    e->handles.clear();
}

const QList<LADSPAPlugin*> &LADSPAHost::plugins() const
{
    return m_plugins;
}

const QList<LADSPAEffect*> &LADSPAHost::effects() const
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
    if(m_effects.isEmpty())
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
