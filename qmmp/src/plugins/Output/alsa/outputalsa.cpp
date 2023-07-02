#include <QSettings>
#include <QSocketNotifier>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include "outputalsa.h"

OutputALSA::OutputALSA()
    : Output()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString dev_name = settings.value("ALSA/device", "default").toString();
    m_use_mmap = settings.value("ALSA/use_mmap", false).toBool();
    pcm_name = strdup(dev_name.toLatin1().data());

    m_alsa_channels[SND_CHMAP_NA]   = Qmmp::CHAN_NULL;
    m_alsa_channels[SND_CHMAP_MONO] = Qmmp::CHAN_FRONT_CENTER;
    m_alsa_channels[SND_CHMAP_FL]   = Qmmp::CHAN_FRONT_LEFT;
    m_alsa_channels[SND_CHMAP_FR]   = Qmmp::CHAN_FRONT_RIGHT;
    m_alsa_channels[SND_CHMAP_RL]   = Qmmp::CHAN_REAR_LEFT;
    m_alsa_channels[SND_CHMAP_RR]   = Qmmp::CHAN_REAR_RIGHT;
    m_alsa_channels[SND_CHMAP_FC]   = Qmmp::CHAN_FRONT_CENTER;
    m_alsa_channels[SND_CHMAP_LFE]  = Qmmp::CHAN_LFE;
    m_alsa_channels[SND_CHMAP_SL]   = Qmmp::CHAN_SIDE_LEFT;
    m_alsa_channels[SND_CHMAP_SR]   = Qmmp::CHAN_SIDE_RIGHT;
    m_alsa_channels[SND_CHMAP_RC]   = Qmmp::CHAN_REAR_CENTER;
}

OutputALSA::~OutputALSA()
{
    uninitialize();
    free(pcm_name);
}

bool OutputALSA::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    m_inited = false;

    if(pcm_handle)
        return false;

    if(snd_pcm_open(&pcm_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0)
    {
        qWarning("OutputALSA: Error opening PCM device %s", pcm_name);
        return false;
    }

    // we need to configure
    uint rate = freq; /* Sample rate */
    uint exact_rate = freq;   /* Sample rate returned by */

    /* load settings from config */
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("ALSA");
    uint buffer_time = settings.value("buffer_time", 500).toUInt()*1000;
    uint period_time = settings.value("period_time", 100).toUInt()*1000;
    bool use_pause = settings.value("use_snd_pcm_pause", false).toBool();
    settings.endGroup();

    snd_pcm_hw_params_t *hwparams = nullptr;
    snd_pcm_sw_params_t *swparams = nullptr;
    int err; //alsa error code

    //hw params
    snd_pcm_hw_params_alloca(&hwparams);
    if((err = snd_pcm_hw_params_any(pcm_handle, hwparams)) < 0)
    {
        qWarning("OutputALSA: Can not read configuration for PCM device: %s", snd_strerror(err));
        return false;
    }

    if(m_use_mmap)
    {
        if((err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_MMAP_INTERLEAVED)) < 0)
        {
            qWarning("OutputALSA: Error setting mmap access: %s", snd_strerror(err));
            m_use_mmap = false;
        }
    }

    if(!m_use_mmap)
    {
        if((err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
        {
            qWarning("OutputALSA: Error setting access: %s", snd_strerror(err));
            return false;
        }
    }

    snd_pcm_format_t alsa_format = SND_PCM_FORMAT_UNKNOWN;
    switch(format)
    {
    case Qmmp::PCM_S8:
        alsa_format = SND_PCM_FORMAT_S8;
        break;
    case Qmmp::PCM_S16LE:
        alsa_format = SND_PCM_FORMAT_S16_LE;
        break;
    case Qmmp::PCM_S24LE:
        alsa_format = SND_PCM_FORMAT_S24_LE;
        break;
    case Qmmp::PCM_S32LE:
        alsa_format = SND_PCM_FORMAT_S32_LE;
        break;
    case Qmmp::PCM_FLOAT:
        alsa_format = SND_PCM_FORMAT_FLOAT;
        break;
    default:
        qWarning("OutputALSA: unsupported format detected");
        return false;
    }

    if((err = snd_pcm_hw_params_set_format(pcm_handle, hwparams, alsa_format)) < 0)
    {
        qDebug("OutputALSA: Error setting format: %s", snd_strerror(err));
        return false;
    }
    exact_rate = rate;

    if((err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, nullptr)) < 0)
    {
        qWarning("OutputALSA: Error setting rate: %s", snd_strerror(err));
        return false;
    }

    if(rate != exact_rate)
    {
        qWarning("OutputALSA: The rate %d Hz is not supported by your hardware.\n==> Using %d Hz instead.", rate, exact_rate);
    }

    uint c = map.count();
    if((err = snd_pcm_hw_params_set_channels_near(pcm_handle, hwparams, &c)) < 0)
    {
        qWarning("OutputALSA: Error setting channels: %s", snd_strerror(err));
        return false;
    }

    if(c != (uint)map.count())
    {
        qWarning("OutputALSA: The channel number %d is not supported by your hardware", map.count());
        qWarning("==> Using %d instead.", c);
    }

    if((err = snd_pcm_hw_params_set_period_time_near(pcm_handle, hwparams, &period_time, nullptr)) < 0)
    {
        qWarning("OutputALSA: Error setting period time: %s", snd_strerror(err));
        return false;
    }

    if((err = snd_pcm_hw_params_set_buffer_time_near(pcm_handle, hwparams, &buffer_time, nullptr)) < 0)
    {
        qWarning("OutputALSA: Error setting buffer time: %s", snd_strerror(err));
        return false;
    }

    if((err = snd_pcm_hw_params(pcm_handle, hwparams)) < 0)
    {
        qWarning("OutputALSA: Error setting HW params: %s", snd_strerror(err));
        return false;
    }
    //read some alsa parameters
    snd_pcm_uframes_t buffer_size = 0;
    snd_pcm_uframes_t period_size = 0;
    if((err = snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size)) < 0)
    {
        qWarning("OutputALSA: Error reading buffer size: %s", snd_strerror(err));
        return false;
    }

    if((err = snd_pcm_hw_params_get_period_size(hwparams, &period_size, nullptr)) < 0)
    {
        qWarning("OutputALSA: Error reading period size: %s", snd_strerror(err));
        return false;
    }
    //swparams
    snd_pcm_sw_params_alloca(&swparams);
    snd_pcm_sw_params_current(pcm_handle, swparams);
    if((err = snd_pcm_sw_params_set_start_threshold(pcm_handle, swparams, buffer_size - period_size)) < 0)
    {
        qWarning("OutputALSA: Error setting threshold: %s", snd_strerror(err));
    }

    if((err = snd_pcm_sw_params(pcm_handle, swparams)) < 0)
    {
        qWarning("OutputALSA: Error setting SW params: %s", snd_strerror(err));
        return false;
    }
    //setup needed values
    m_chunk_size = period_size;
    m_can_pause = snd_pcm_hw_params_can_pause(hwparams) && use_pause;
    qDebug("OutputALSA: can pause: %d", m_can_pause);

    ChannelMap out_map = map;
    //channel map configuration
    snd_pcm_chmap_t *chmap = snd_pcm_get_chmap(pcm_handle);
    if(chmap)
    {
        out_map.clear();
        char tmp[256];
        memset(tmp,0,256);
        snd_pcm_chmap_print(chmap, 256, tmp);
        qDebug("OutputALSA: received channel map: %s",tmp);

        for(uint i = 0; i < chmap->channels; ++i)
        {
            if(m_alsa_channels.contains(chmap->pos[i]))
                out_map.append(m_alsa_channels.value(chmap->pos[i]));
            else
                out_map.append(Qmmp::CHAN_NULL);
        }
        free(chmap);
    }
    else
    {
        qWarning("OutputALSA: Unable to receive current channel map");
    }

    configure(exact_rate, out_map, format); //apply configuration
    //create alsa prebuffer;
    m_prebuf_size = 2 * snd_pcm_frames_to_bytes(pcm_handle, m_chunk_size); //buffer for two periods
    m_prebuf = (uchar *)malloc(m_prebuf_size);
    m_inited = true;
    return true;
}

qint64 OutputALSA::latency()
{
    snd_pcm_sframes_t delay = 0;
    snd_pcm_delay(pcm_handle, &delay);
    delay = qBound(3000L, delay, 30000L); //filter out possible invalid values
    return m_prebuf_fill * 1000 / sampleRate() / channels() / sampleSize() + delay * 1000 / sampleRate();
}

void OutputALSA::drain()
{
    snd_pcm_uframes_t l = snd_pcm_bytes_to_frames(pcm_handle, m_prebuf_fill);
    while(l > 0)
    {
        long m = alsa_write(m_prebuf, l);
        if(m >= 0)
        {
            l -= m;
            m = snd_pcm_frames_to_bytes(pcm_handle, m); // convert frames to bytes
            m_prebuf_fill -= m;
            memmove(m_prebuf, m_prebuf + m, m_prebuf_fill);
        }
        else
        {
            break;
        }
    }
    snd_pcm_nonblock(pcm_handle, 0);
    snd_pcm_drain(pcm_handle);
    snd_pcm_nonblock(pcm_handle, 1);
}

void OutputALSA::reset()
{
    m_prebuf_fill = 0;
    snd_pcm_drop(pcm_handle);
    snd_pcm_prepare(pcm_handle);
}

void OutputALSA::suspend()
{
    if(m_can_pause)
        snd_pcm_pause(pcm_handle, 1);
    snd_pcm_prepare(pcm_handle);
}

void OutputALSA::resume()
{
    if(m_can_pause)
        snd_pcm_pause(pcm_handle, 0);
    snd_pcm_prepare(pcm_handle);
}

qint64 OutputALSA::writeAudio(unsigned char *data, qint64 maxSize)
{
    if((maxSize = qMin(maxSize, m_prebuf_size - m_prebuf_fill)) > 0)
    {
        memmove(m_prebuf + m_prebuf_fill, data, maxSize);
        m_prebuf_fill += maxSize;
    }

    snd_pcm_uframes_t l = snd_pcm_bytes_to_frames(pcm_handle, m_prebuf_fill);

    while(l >= m_chunk_size)
    {
        snd_pcm_wait(pcm_handle, 10);
        long m;
        if((m = alsa_write(m_prebuf, m_chunk_size)) >= 0)
        {
            l -= m;
            m = snd_pcm_frames_to_bytes(pcm_handle, m); // convert frames to bytes
            m_prebuf_fill -= m;
            memmove(m_prebuf, m_prebuf + m, m_prebuf_fill); //move data to begin
        }
        else
            return -1;
    }
    return maxSize;
}

long OutputALSA::alsa_write(unsigned char *data, long size)
{
    long m = snd_pcm_avail_update(pcm_handle);
    if(m >= 0 && m < size)
    {
        snd_pcm_wait(pcm_handle, 500);
        return 0;
    }

    if(m_use_mmap)
        m = snd_pcm_mmap_writei (pcm_handle, data, size);
    else
        m = snd_pcm_writei (pcm_handle, data, size);

    if(m == -EAGAIN)
    {
        snd_pcm_wait(pcm_handle, 500);
        return 0;
    }
    else if(m >= 0)
    {
        if(m < size)
        {
            snd_pcm_wait(pcm_handle, 500);
        }
        return m;
    }
    else if(m == -EPIPE)
    {
        qDebug("OutputALSA: buffer underrun!");
        if((m = snd_pcm_prepare(pcm_handle)) < 0)
        {
            qDebug("OutputALSA: Can't recover after underrun: %s",
                    snd_strerror(m));
            /* TODO: reopen the device */
            return -1;
        }
        return 0;
    }
#ifdef ESTRPIPE
    else if(m == -ESTRPIPE)
    {
        qDebug("OutputALSA: Suspend, trying to resume");
        while((m = snd_pcm_resume(pcm_handle))
                == -EAGAIN)
            sleep (1);
        if(m < 0)
        {
            qDebug("OutputALSA: Failed, restarting");
            if((m = snd_pcm_prepare(pcm_handle)) < 0)
            {
                qDebug("OutputALSA: Failed to restart device: %s.",
                        snd_strerror(m));
                return -1;
            }
        }
        return 0;
    }
#endif
    qDebug("OutputALSA: error: %s", snd_strerror(m));
    return snd_pcm_prepare (pcm_handle);
}

void OutputALSA::uninitialize()
{
    if(!m_inited)
        return;
    m_inited = false;
    if(pcm_handle)
    {
        snd_pcm_drop(pcm_handle);
        qDebug("OutputALSA: closing pcm_handle");
        snd_pcm_close(pcm_handle);
        pcm_handle = nullptr;
    }
    if(m_prebuf)
        free(m_prebuf);
    m_prebuf = nullptr;
}
/* ****** MIXER ******* */

VolumeALSA::VolumeALSA()
{
    //alsa mixer
    m_mixer = nullptr;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    QString card = settings.value("ALSA/mixer_card", "hw:0").toString();
    QString dev = settings.value("ALSA/mixer_device", "PCM").toString();
    setupMixer(card, dev);
}


VolumeALSA::~VolumeALSA()
{
    if(m_mixer)
        snd_mixer_close(m_mixer);
}

void VolumeALSA::setVolume(const VolumeSettings &vol)
{
    if(!pcm_element)
        return;

    snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, vol.left);
    snd_mixer_selem_set_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, vol.right);
}

VolumeSettings VolumeALSA::volume() const
{
    VolumeSettings vol;

    if(pcm_element)
    {
        long value = 0;
        snd_mixer_handle_events(m_mixer);
        snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_LEFT, &value);
        vol.left = value;
        snd_mixer_selem_get_playback_volume(pcm_element, SND_MIXER_SCHN_FRONT_RIGHT, &value);
        vol.right = value;
    }
    return vol;
}

Volume::VolumeFlags VolumeALSA::flags() const
{
    return Volume::HasNotifySignal;
}

int VolumeALSA::setupMixer(QString card, QString device)
{
    char *name;
    int err, index;
    pcm_element = nullptr;

    qDebug("OutputALSA: setupMixer()");

    if((err = getMixer(&m_mixer, card)) < 0)
        return err;

    parseMixerName(device.toLatin1().data(), &name, &index);

    pcm_element = getMixerElem(m_mixer, name, index);

    free(name);

    if(!pcm_element)
    {
        qWarning("OutputALSA: Failed to find mixer element");
        return -1;
    }

    if((err = snd_mixer_selem_set_playback_volume_range(pcm_element, 0, 100)) < 0)
    {
        qWarning("OutputALSA: Unable to set volume range: %s", snd_strerror(-err));
        pcm_element = nullptr;
        return -1;
    }

    // setup socket notifiers to monitor the state changes of the mixer
    int n = snd_mixer_poll_descriptors_count(m_mixer);
    if(n > 0)
    {
        struct pollfd* fds = new struct pollfd[n];
        n = snd_mixer_poll_descriptors(m_mixer, fds, n);
        for(int i = 0; i < n; ++i)
        {
            int sock = fds[i].fd;
            QSocketNotifier* sn = new QSocketNotifier(sock, QSocketNotifier::Read, this);
            connect(sn, SIGNAL(activated(int)), SIGNAL(changed()));
        }
        delete[] fds;
    }

    qDebug("OutputALSA: setupMixer() success");
    return 0;
}


void VolumeALSA::parseMixerName(char *str, char **name, int *index)
{
    char *end;

    while(isspace(*str))
        str++;

    if((end = strchr(str, ',')) != nullptr)
    {
        *name = strndup(str, end - str);
        end++;
        *index = atoi(end);
    }
    else
    {
        *name = strdup(str);
        *index = 0;
    }
}

snd_mixer_elem_t* VolumeALSA::getMixerElem(snd_mixer_t *mixer, char *name, int index)
{
    snd_mixer_selem_id_t* selem_id;
    snd_mixer_elem_t* elem;
    snd_mixer_selem_id_alloca(&selem_id);

    if(index != -1)
        snd_mixer_selem_id_set_index(selem_id, index);
    if(name != nullptr)
        snd_mixer_selem_id_set_name(selem_id, name);

    elem = snd_mixer_find_selem(mixer, selem_id);

    return elem;
}

int VolumeALSA::getMixer(snd_mixer_t **mixer, QString card)
{
    int err;

    if((err = snd_mixer_open(mixer, 0)) < 0)
    {
        qWarning("OutputALSA: Failed to open empty mixer: %s",
                 snd_strerror(-err));
        mixer = nullptr;
        return -1;
    }

    if((err = snd_mixer_attach(*mixer, card.toLatin1().constData())) < 0)
    {
        qWarning("OutputALSA: Attaching to mixer %s failed: %s",
                 qPrintable(card), snd_strerror(-err));
        return -1;
    }
    if((err = snd_mixer_selem_register(*mixer, nullptr, nullptr)) < 0)
    {
        qWarning("OutputALSA: Failed to register mixer: %s",
                 snd_strerror(-err));
        return -1;
    }
    if((err = snd_mixer_load(*mixer)) < 0)
    {
        qWarning("OutputALSA: Failed to load mixer: %s",
                 snd_strerror(-err));
        return -1;
    }
    return (*mixer != nullptr);
}

