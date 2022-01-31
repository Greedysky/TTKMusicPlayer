#include "wildmidihelper.h"
extern "C" {
#include <wildmidi_lib.h>
}
#include <QFile>
#include <QSettings>
#include <qmmp/qmmp.h>

WildMidiHelper *WildMidiHelper::m_instance = nullptr;

WildMidiHelper::WildMidiHelper(QObject *parent)
    : QObject(parent)
{
    m_instance = this;
}

WildMidiHelper::~WildMidiHelper()
{
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

    WildMidi_Shutdown();

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

    m_sampleRate = sample_rate;
    if(WildMidi_Init(QmmpPrintable(conf_path), sample_rate, mixer_options) < 0)
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

    m_inited = false;
    m_mutex.unlock();
    initialize();
}

QString WildMidiHelper::configFile() const
{
    const QString path = Qmmp::pluginPath() + "/../GPlugins/config/wildmidi.cfg";
    return QFile::exists(path) ? path : QString();
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

WildMidiHelper *WildMidiHelper::instance()
{
    return m_instance;
}

int WildMidiHelper::bitrate() const
{
    return 8;
}

int WildMidiHelper::sampleRate() const
{
    return m_sampleRate;
}

int WildMidiHelper::channels() const
{
    return 2;
}

int WildMidiHelper::bitsPerSample() const
{
    return 16;
}
