#include "outputpipewire.h"

#include <cmath>
#include <QSettings>
#include <spa/param/props.h>

#if !PW_CHECK_VERSION(0,3,33)
  #define PW_KEY_NODE_RATE "node.rate"
#endif

OutputPipeWire *OutputPipeWire::instance = nullptr;
VolumePipeWire *OutputPipeWire::volumeControl = nullptr;

OutputPipeWire::OutputPipeWire(): Output()
{
    m_pw_channels[Qmmp::CHAN_NULL] = SPA_AUDIO_CHANNEL_NA;
    m_pw_channels[Qmmp::CHAN_FRONT_CENTER] = SPA_AUDIO_CHANNEL_MONO;
    m_pw_channels[Qmmp::CHAN_FRONT_LEFT] = SPA_AUDIO_CHANNEL_FL;
    m_pw_channels[Qmmp::CHAN_FRONT_RIGHT] = SPA_AUDIO_CHANNEL_FR;
    m_pw_channels[Qmmp::CHAN_REAR_LEFT] = SPA_AUDIO_CHANNEL_RL;
    m_pw_channels[Qmmp::CHAN_REAR_RIGHT] = SPA_AUDIO_CHANNEL_RR;
    m_pw_channels[Qmmp::CHAN_FRONT_CENTER] = SPA_AUDIO_CHANNEL_FC;
    m_pw_channels[Qmmp::CHAN_LFE] = SPA_AUDIO_CHANNEL_LFE;
    m_pw_channels[Qmmp::CHAN_SIDE_LEFT] = SPA_AUDIO_CHANNEL_SL;
    m_pw_channels[Qmmp::CHAN_SIDE_RIGHT] = SPA_AUDIO_CHANNEL_SR;
    m_pw_channels[Qmmp::CHAN_REAR_CENTER] = SPA_AUDIO_CHANNEL_RC;

    instance = this;
    pw_init(nullptr, nullptr);
}

OutputPipeWire::~OutputPipeWire()
{
    uninitialize();
    pw_deinit();
    instance = nullptr;
}

void OutputPipeWire::onStateChanged(void *data, enum pw_stream_state old,
                                    enum pw_stream_state state, const char *error)
{
    Q_UNUSED(old);
    Q_UNUSED(error);

    OutputPipeWire *o = static_cast<OutputPipeWire*>(data);

    if(o->m_ignoreStateChange)
    {
        return;
    }

    if(state == PW_STREAM_STATE_UNCONNECTED ||
       state == PW_STREAM_STATE_PAUSED ||
       state == PW_STREAM_STATE_STREAMING)
    {
        pw_thread_loop_signal(o->m_loop, false);

        if(state == PW_STREAM_STATE_STREAMING && volumeControl)
        {
            instance->setMuted(volumeControl->isMuted());
        }
    }
}

bool OutputPipeWire::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    static const pw_core_events coreEvents = {
        .version = PW_VERSION_CORE_EVENTS,
        .done = OutputPipeWire::onCoreEventDone
    };

    static const pw_registry_events registryEvents = {
        .version = PW_VERSION_REGISTRY_EVENTS,
        .global = OutputPipeWire::onRegistryEventGlobal
    };

    static const struct pw_stream_events streamEvents = {
        .version = PW_VERSION_STREAM_EVENTS,
        .state_changed = OutputPipeWire::onStateChanged,
        .process = OutputPipeWire::onProcess,
        .drained = OutputPipeWire::onDrained
    };

    if(!(m_loop = pw_thread_loop_new("pipewire-main-loop", nullptr)))
    {
        qWarning("unable to create main loop");
        return false;
    }

    if(!(m_context = pw_context_new(pw_thread_loop_get_loop(m_loop), nullptr, 0)))
    {
        qWarning("unable to create context");
        return false;
    }

    if(!(m_core = pw_context_connect(m_context, nullptr, 0)))
    {
        qWarning("unable to connect context");
        return false;
    }
    pw_core_add_listener(m_core, &m_coreListener, &coreEvents, this);

    if(!(m_registry = pw_core_get_registry(m_core, PW_VERSION_REGISTRY, 0)))
    {
        qWarning("unable to get registry interface");
        return false;
    }
    pw_registry_add_listener(m_registry, &m_registryListener, &registryEvents, this);

    m_coreInitSeq = pw_core_sync(m_core, PW_ID_CORE, m_coreInitSeq);

    if(pw_thread_loop_start(m_loop) != 0)
    {
        qWarning("unable to start loop");
        return false;
    }

    pw_thread_loop_lock(m_loop);
    while(!m_inited)
    {
        if(pw_thread_loop_timed_wait(m_loop, 2) != 0)
        {
            break;
        }
    }
    pw_thread_loop_unlock(m_loop);

    if(!m_inited || !m_hasSinks)
    {
        qWarning("unable to initialize loop");
        return false;
    }

    m_stride = AudioParameters::sampleSize(format) * map.count();
    m_frames = qBound(64, static_cast<int>(std::ceil(2048.0 * freq / 48000.0)), 8192);
    m_bufferSize = m_frames * m_stride;
    m_buffer = new unsigned char[m_bufferSize];

    pw_properties *props = pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio",
                                             PW_KEY_MEDIA_CATEGORY, "Playback",
                                             PW_KEY_MEDIA_ROLE, "Music",
                                             PW_KEY_MEDIA_ICON_NAME, "qmmp",
                                             nullptr);
    pw_properties_setf(props, PW_KEY_NODE_RATE, "1/%u", freq);
    pw_properties_setf(props, PW_KEY_NODE_LATENCY, "%u/%u", m_frames, freq);

    pw_thread_loop_lock(m_loop);

    if(!(m_stream = pw_stream_new(m_core, "Playback", props)))
    {
        qWarning("unable to create stream");
        pw_thread_loop_unlock(m_loop);
        return false;
    }

    m_streamListener = {};
    pw_stream_add_listener(m_stream, &m_streamListener, &streamEvents, this);

    uint8_t buffer[1024];
    spa_pod_builder b = {
        .data = buffer,
        .size = sizeof(buffer)
    };

    spa_audio_info_raw info = {
        .format = SPA_AUDIO_FORMAT_F32,
        .flags = SPA_AUDIO_FLAG_NONE,
        .rate = freq,
        .channels = static_cast<unsigned int>(map.count())
    };

    switch(format)
    {
        case Qmmp::PCM_S8:
            info.format = SPA_AUDIO_FORMAT_S8;
            break;
        case Qmmp::PCM_S16LE:
            info.format = SPA_AUDIO_FORMAT_S16_LE;
            break;
        case Qmmp::PCM_S24LE:
            info.format = SPA_AUDIO_FORMAT_S24_32_LE;
            break;
        case Qmmp::PCM_S32LE:
            info.format = SPA_AUDIO_FORMAT_S32_LE;
            break;
        case Qmmp::PCM_FLOAT:
            info.format = SPA_AUDIO_FORMAT_F32_LE;
            break;
        default:
            info.format = SPA_AUDIO_FORMAT_S16_LE;
    }

    for(int i = 0; i < map.count(); i++)
    {
        info.position[i] = m_pw_channels[map.value(i)];
    }

    const spa_pod *params[2];
    params[0] = spa_format_audio_raw_build(
                &b,
                SPA_PARAM_EnumFormat,
                &info
                );
    /*params[1] = static_cast<spa_pod*>(spa_pod_builder_add_object(
                                                &b,
                                                SPA_TYPE_OBJECT_ParamBuffers, SPA_PARAM_Buffers,
                                                SPA_PARAM_BUFFERS_size, m_bufferSize)
                                                );*/

    pw_stream_flags streamFlags = static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT |
                                                               PW_STREAM_FLAG_MAP_BUFFERS |
                                                               PW_STREAM_FLAG_RT_PROCESS);

    if(pw_stream_connect(m_stream, PW_DIRECTION_OUTPUT, PW_ID_ANY, streamFlags, params, 1) != 0)
    {
        pw_thread_loop_unlock(m_loop);
        qDebug("unable to connect stream");
        return false;
    }

    Output::configure(freq, map, format);
    pw_thread_loop_unlock(m_loop);
    qDebug("ready");
    return true;
}

qint64 OutputPipeWire::latency()
{
    return (m_buffer_at / m_stride + m_frames) * 1000 / sampleRate();
}

qint64 OutputPipeWire::writeAudio(unsigned char *data, qint64 maxSize)
{
    pw_thread_loop_lock(m_loop);

    if(m_buffer_at == m_bufferSize)
    {
        if (pw_thread_loop_timed_wait(m_loop, 1) != 0)
        {
            pw_thread_loop_unlock(m_loop);
            return 0;
        }
    }

    const qint64 size = qMin(qint64(m_bufferSize - m_buffer_at), maxSize);
    memcpy(m_buffer + m_buffer_at, data, size);
    m_buffer_at += size;
    pw_thread_loop_unlock(m_loop);
    return size;
}

void OutputPipeWire::drain()
{
    pw_thread_loop_lock(m_loop);
    if(m_buffer_at > 0)
    {
        pw_thread_loop_timed_wait(m_loop, 2);
    }

    pw_stream_flush(m_stream, true);
    pw_thread_loop_timed_wait(m_loop, 2);
    pw_thread_loop_unlock(m_loop);
}

void OutputPipeWire::reset()
{
    pw_thread_loop_lock(m_loop);
    m_buffer_at = 0;
    pw_thread_loop_unlock(m_loop);
    pw_stream_flush(m_stream, false);
}

void OutputPipeWire::suspend()
{
    pw_thread_loop_lock(m_loop);
    pw_stream_set_active(m_stream, false);
    pw_thread_loop_unlock(m_loop);
}

void OutputPipeWire::resume()
{
    pw_thread_loop_lock(m_loop);
    pw_stream_set_active(m_stream, true);
    pw_thread_loop_unlock(m_loop);
}

void OutputPipeWire::setMuted(bool mute)
{
    setVolume(mute ? VolumeSettings() : volumeControl->volume());
}

void OutputPipeWire::setVolume(const VolumeSettings &v)
{
    pw_thread_loop_lock(m_loop);

    float* values = new float[channels()];

    if(channels() == 2)
    {
        values[0] = float(v.left) / 100.0;
        values[1] = float(v.right) / 100.0;
    }
    else
    {
        for(int i = 0; i < channels(); ++i)
        {
            values[i] = float(qMax(v.left, v.right)) / 100.0;
        }
    }

    pw_stream_set_control(m_stream, SPA_PROP_channelVolumes, channels(), values, 0);
    delete [] values;
    pw_thread_loop_unlock(m_loop);
}

void OutputPipeWire::uninitialize()
{
    if(m_stream)
    {
        pw_thread_loop_lock(m_loop);
        m_ignoreStateChange = true;
        pw_stream_disconnect(m_stream);
        pw_stream_destroy(m_stream);
        m_ignoreStateChange = false;
        m_stream = nullptr;
        pw_thread_loop_unlock(m_loop);
    }

    if(m_loop)
    {
        pw_thread_loop_stop(m_loop);
    }

    if(m_registry)
    {
        pw_proxy_destroy(reinterpret_cast<pw_proxy*>(m_registry));
        m_registry = nullptr;
    }

    if(m_core)
    {
        pw_core_disconnect(m_core);
        m_core = nullptr;
    }

    if(m_context)
    {
        pw_context_destroy(m_context);
        m_context = nullptr;
    }

    if(m_loop)
    {
        pw_thread_loop_destroy(m_loop);
        m_loop = nullptr;
    }

    if(m_buffer)
    {
        delete [] m_buffer;
        m_buffer = nullptr;
    }
}

//callbacks
void OutputPipeWire::onProcess(void *data)
{
    OutputPipeWire *o = static_cast<OutputPipeWire*>(data);

    if(!o->m_buffer_at)
    {
        pw_thread_loop_signal(o->m_loop, false);
        return;
    }

    struct pw_buffer *b = pw_stream_dequeue_buffer(o->m_stream);
    struct spa_buffer *buf = b->buffer;

    uint32_t size = qMin(buf->datas[0].maxsize, uint32_t(o->m_buffer_at));
    memcpy(buf->datas[0].data, o->m_buffer, size);
    o->m_buffer_at -= size;
    memmove(o->m_buffer, o->m_buffer + size, o->m_buffer_at);

    b->buffer->datas[0].chunk->offset = 0;
    b->buffer->datas[0].chunk->size = o->m_bufferSize;
    b->buffer->datas[0].chunk->stride = o->m_stride;

    pw_stream_queue_buffer(o->m_stream, b);
    pw_thread_loop_signal(o->m_loop, false);
}

void OutputPipeWire::onDrained(void *data)
{
    OutputPipeWire *o = static_cast<OutputPipeWire*>(data);
    pw_thread_loop_signal(o->m_loop, false);
}

void OutputPipeWire::onCoreEventDone(void *data, uint32_t id, int seq)
{
    OutputPipeWire *o = static_cast<OutputPipeWire*>(data);

    if (id == PW_ID_CORE && seq == o->m_coreInitSeq)
    {
        spa_hook_remove(&o->m_registryListener);
        spa_hook_remove(&o->m_coreListener);

        o->m_inited = true;
        pw_thread_loop_signal(o->m_loop, false);
    }
}

void OutputPipeWire::onRegistryEventGlobal(void *data, uint32_t id, uint32_t permissions, const char *type, uint32_t version, const spa_dict *props)
{
    Q_UNUSED(id)
    Q_UNUSED(permissions)
    Q_UNUSED(version)

    OutputPipeWire *o = static_cast<OutputPipeWire*>(data);

    if(qstrcmp(type, PW_TYPE_INTERFACE_Node) != 0)
    {
        return;
    }

    auto media_class = spa_dict_lookup(props, PW_KEY_MEDIA_CLASS);
    if(!media_class)
    {
        return;
    }

    if(qstrcmp(media_class, "Audio/Sink") != 0)
    {
        return;
    }

    o->m_hasSinks = true;
    o->m_coreInitSeq = pw_core_sync(o->m_core, PW_ID_CORE, o->m_coreInitSeq);
}

//volume control
VolumePipeWire::VolumePipeWire()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_volume.left = settings.value("OutputPipeWire/left_volume", 100).toInt();
    m_volume.right = settings.value("OutputPipeWire/right_volume", 100).toInt();
    OutputPipeWire::volumeControl = this;
}

VolumePipeWire::~VolumePipeWire()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("OutputPipeWire/left_volume", m_volume.left);
    settings.setValue("OutputPipeWire/right_volume", m_volume.right);
    OutputPipeWire::volumeControl = nullptr;
}

void VolumePipeWire::setVolume(const VolumeSettings &vol)
{
    if(OutputPipeWire::instance)
    {
        OutputPipeWire::instance->setVolume(vol);
    }
    m_volume = vol;
}

VolumeSettings VolumePipeWire::volume() const
{
    return m_volume;
}

bool VolumePipeWire::isMuted() const
{
    return m_muted;
}

void VolumePipeWire::setMuted(bool mute)
{
    if(OutputPipeWire::instance)
    {
        OutputPipeWire::instance->setMuted(mute);
    }
    m_muted = mute;
}

Volume::VolumeFlags VolumePipeWire::flags() const
{
    return Volume::IsMuteSupported | Volume::HasNotifySignal;
}
