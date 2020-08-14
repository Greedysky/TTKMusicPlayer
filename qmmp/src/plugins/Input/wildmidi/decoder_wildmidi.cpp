#include <stdint.h>
#include "wildmidihelper.h"
#include "decoder_wildmidi.h"

DecoderWildMidi::DecoderWildMidi(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderWildMidi::~DecoderWildMidi()
{
    if(midi_ptr)
    {
        WildMidiHelper::instance()->removePtr(midi_ptr);
        WildMidi_Close(midi_ptr);
    }
}

bool DecoderWildMidi::initialize()
{
    m_totalTime = 0;

    if(!WildMidiHelper::instance()->initialize())
    {
        qWarning("DecoderWildMidi: initialization failed");
        return false;
    }
    WildMidiHelper::instance()->readSettings();
    midi_ptr = WildMidi_Open (m_path.toLocal8Bit().constData());

    if(!midi_ptr)
    {
        qWarning("DecoderWildMidi: unable to open file");
        return false;
    }
    WildMidiHelper::instance()->addPtr(midi_ptr);


    m_sample_rate = WildMidiHelper::instance()->sampleRate();
    _WM_Info *wm_info = WildMidi_GetInfo(midi_ptr);
    m_totalTime = (qint64)wm_info->approx_total_samples * 1000 / WildMidiHelper::instance()->sampleRate();
    configure(m_sample_rate, 2, Qmmp::PCM_S16LE);
    qDebug("DecoderWildMidi: initialize succes");
    return true;
}

qint64 DecoderWildMidi::totalTime() const
{
    return m_totalTime;
}

void DecoderWildMidi::seek(qint64 pos)
{
    ulong sample = (ulong)m_sample_rate * pos / 1000;
    WildMidi_FastSeek(midi_ptr, &sample);
}

int DecoderWildMidi::bitrate() const
{
    return 8;
}

qint64 DecoderWildMidi::read(unsigned char *data, qint64 size)
{
#if defined(LIBWILDMIDI_VERSION) && (LIBWILDMIDI_VERSION >= 0x000400)
    return WildMidi_GetOutput (midi_ptr, (int8_t *)data, size);
#else
    return WildMidi_GetOutput (midi_ptr, (char *)data, size);
#endif

}
