#include "adlmidihelper.h"

#include <QSettings>

AdlMidiHelper::AdlMidiHelper(const QString &path)
    : m_path(path)
{

}

AdlMidiHelper::~AdlMidiHelper()
{
    deinit();
}

void AdlMidiHelper::deinit()
{
    if(m_input)
    {
        adl_close(m_input);
    }
}

bool AdlMidiHelper::initialize()
{
    m_input = adl_init(sampleRate());
    if(adl_openFile(m_input, QmmpPrintable(m_path)) < 0)
    {
        qWarning("AdlMidiHelper: adl_openFile error");
        return false;
    }

    readSettings();
    adl_setLoopEnabled(m_input, 0);
    return true;
}

void AdlMidiHelper::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    if(!settings.value("use_wopl", false).toBool())
    {
        const int index = settings.value("index", -1).toInt();
        adl_setBank(m_input, index >= adl_getBanksCount() ? 0 : index);
    }
    else
    {
        adl_openBankFile(m_input, QmmpPrintable(settings.value("conf_path").toString()));
    }
    settings.endGroup();
    adl_reset(m_input);
}

qint64 AdlMidiHelper::read(unsigned char *data, qint64 maxSize)
{
    const int samples = maxSize / channels();
    return adl_play(m_input, samples, (short*)data) * channels();
}

QString AdlMidiHelper::trackTitles() const
{
    QString value;
    for(int i = 0; i < trackTitleCount(); ++i)
    {

        value += adl_metaTrackTitle(m_input, i);
        value += "\n";
    }
    return value;
}

QString AdlMidiHelper::metaMarkers() const
{
    QString value;
    for(int i = 0; i < metaMarkerCount(); ++i)
    {

        value += adl_metaMarker(m_input, i).label;
        value += "\n";
    }
    return value;
}
