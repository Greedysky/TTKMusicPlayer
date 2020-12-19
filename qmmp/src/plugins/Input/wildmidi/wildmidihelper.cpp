#include <QSettings>
#include <QFile>
extern "C"{
#include <wildmidi_lib.h>
}
#include <qmmp/qmmp.h>
#include "wildmidihelper.h"

WildMidiHelper *WildMidiHelper::m_instance = nullptr;

WildMidiHelper::WildMidiHelper(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
}

WildMidiHelper::~WildMidiHelper()
{
    if(m_inited)
        WildMidi_Shutdown();
    m_instance = nullptr;
}

bool WildMidiHelper::initialize()
{
    m_mutex.lock();
    if(m_inited)
    {
        m_mutex.unlock();
        return true;
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    unsigned short int mixer_options = 0;
    QString conf_path = configFile();
    conf_path = settings.value("conf_path", conf_path).toString();
    if(conf_path.isEmpty() || !QFile::exists(conf_path))
    {
        qWarning("WildMidiHelper: invalid config path: %s", qPrintable(conf_path));
        m_mutex.unlock();
        return false;
    }
    unsigned short int sample_rate = settings.value("sample_rate", 44100).toInt();
    if(settings.value("enhanced_resampling", false).toBool())
        mixer_options |= WM_MO_ENHANCED_RESAMPLING;
    if(settings.value("reverberation", false).toBool())
        mixer_options |= WM_MO_REVERB;
    settings.endGroup();

    m_sample_rate = sample_rate;
    if(WildMidi_Init(qPrintable(conf_path), sample_rate, mixer_options) < 0)
    {
        qWarning("WildMidiHelper: unable to initialize WildMidi library, %s", WildMidi_GetError());
        m_mutex.unlock();
        return false;
    }

    m_inited = true;
    m_mutex.unlock();
    return true;
}

void WildMidiHelper::readSettings()
{
    m_mutex.lock();
    if(!m_ptrs.isEmpty())
    {
        m_mutex.unlock();
        return;
    }
    if(m_inited)
        WildMidi_Shutdown();
    m_inited = false;
    m_mutex.unlock();
    initialize();
}

void WildMidiHelper::addPtr(void *t)
{
    m_mutex.lock();
    m_ptrs.append(t);
    m_mutex.unlock();
}

void WildMidiHelper::removePtr(void *t)
{
    m_mutex.lock();
    m_ptrs.removeAll(t);
    m_mutex.unlock();
}

QString WildMidiHelper::configFile() const
{
    const QString path = Qmmp::pluginPath() + "/../MPlugins/config/wildmidi.cfg";
    return QFile::exists(path) ? path : QString();
}

quint32 WildMidiHelper::sampleRate()
{
    return m_sample_rate;
}

WildMidiHelper *WildMidiHelper::instance()
{
    return m_instance;
}
