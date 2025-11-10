#include "outputoss4.h"

#include <QSettings>

OutputOSS4 *OutputOSS4::m_instance = nullptr;
VolumeOSS4 *OutputOSS4::m_vc = nullptr;
Qmmp::ChannelPosition OutputOSS4::m_oss_pos[16] =
{
    Qmmp::CHAN_NULL,         //0 = null
    Qmmp::CHAN_FRONT_LEFT,   //1 = left
    Qmmp::CHAN_FRONT_RIGHT,  //2 = right
    Qmmp::CHAN_FRONT_CENTER, //3 = center
    Qmmp::CHAN_LFE,          //4 = lfe
    Qmmp::CHAN_SIDE_LEFT,    //5 = left surround
    Qmmp::CHAN_SIDE_RIGHT,   //6 = right surround
    Qmmp::CHAN_REAR_LEFT,    //7 = left rear
    Qmmp::CHAN_REAR_LEFT,    //8 = right rear
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL,
    Qmmp::CHAN_NULL
};


OutputOSS4::OutputOSS4()
    : Output()
{
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_audio_device = settings.value("OSS4/device", DEFAULT_DEV).toString();
}

OutputOSS4::~OutputOSS4()
{
    if(m_audio_fd >= 0)
    {
        ioctl(m_audio_fd, SNDCTL_DSP_RESET, 0);
        close(m_audio_fd);
        m_audio_fd = -1;
    }
    m_instance = nullptr;
}

int OutputOSS4::fd() const
{
    return m_audio_fd;
}

OutputOSS4 *OutputOSS4::instance()
{
    return m_instance;
}

void OutputOSS4::post() const
{
    ioctl(m_audio_fd, SNDCTL_DSP_POST, 0);
}

void OutputOSS4::sync() const
{
    ioctl(m_audio_fd, SNDCTL_DSP_SYNC, 0);
}

bool OutputOSS4::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    m_audio_fd = open(m_audio_device.toLatin1().constData(), O_WRONLY);

    if(m_audio_fd < 0)
    {
        qWarning("OSS4Output: unable to open output device '%s'; error: %s",
                 qPrintable(m_audio_device), strerror(errno));
        return false;
    }

    ioctl(m_audio_fd, SNDCTL_DSP_RESET, 0);

    int p;
    int chan = map.count();
    switch(format)
    {
#ifdef AFMT_FLOAT
    case Qmmp::PCM_FLOAT:
        p = AFMT_FLOAT;
        break;
#endif
    case Qmmp::PCM_S32LE:
        p = AFMT_S32_LE;
        break;
    case Qmmp::PCM_S32BE:
        p = AFMT_S32_BE;
        break;
    case Qmmp::PCM_S24LE:
        p = AFMT_S24_LE;
        break;
    case Qmmp::PCM_S24BE:
        p = AFMT_S24_BE;
        break;
    case Qmmp::PCM_S16LE:
        p = AFMT_S16_LE;
        break;
    case Qmmp::PCM_S16BE:
        p = AFMT_S16_BE;
        break;
    case Qmmp::PCM_S8:
        p = AFMT_S8;
        break;
    default:
        qWarning("OutputOSS4: unsupported audio format");
        return false;
    }

    if(ioctl(m_audio_fd, SNDCTL_DSP_SETFMT, &p) == -1)
        qWarning("OutputOSS4: ioctl SNDCTL_DSP_SETFMT failed: %s",strerror(errno));

    if(ioctl(m_audio_fd, SNDCTL_DSP_CHANNELS, &chan) == -1)
        qWarning("OutputOSS4: ioctl SNDCTL_DSP_CHANNELS failed: %s", strerror(errno));

    if(ioctl(m_audio_fd, SNDCTL_DSP_SPEED, &freq) < 0)
        qWarning("OutputOSS4: ioctl SNDCTL_DSP_SPEED failed: %s", strerror(errno));

    int enabled = 1;
    if(ioctl(m_audio_fd, SNDCTL_DSP_COOKEDMODE, &enabled) == -1)
        qWarning("OutputOSS4: ioctl SNDCTL_DSP_COOKEDMODE: %s", strerror(errno));

    quint64 layout = 0;
    if(ioctl (m_audio_fd, SNDCTL_DSP_GET_CHNORDER, &layout) == -1)
    {
        qWarning("OutputOSS4: couldn't query channel layout, assuming default");
        layout = CHNORDER_NORMAL;
    }

    ChannelMap oss_map;
    for(int i = 0; i < chan; ++i)
    {
        quint32 pos = ((layout >> (i * 4)) & 0x0f);
        oss_map << m_oss_pos[pos];
    }

    ioctl(m_audio_fd, SNDCTL_DSP_RESET, 0);

    configure(freq, oss_map, format);

    if(m_vc)
        m_vc->restore();
    return true;
}

qint64 OutputOSS4::latency()
{
    return 0;
}

qint64 OutputOSS4::writeAudio(unsigned char *data, qint64 maxSize)
{
    qint64 m = write(m_audio_fd, data, maxSize);
    post();
    return m;
}

void OutputOSS4::drain()
{
    ioctl(m_audio_fd, SNDCTL_DSP_SYNC, 0);
}

void OutputOSS4::reset()
{
    ioctl(m_audio_fd, SNDCTL_DSP_SKIP, 0);
}


VolumeOSS4::VolumeOSS4()
{
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_volume = settings.value("OSS4/volume", 0x3232).toInt();
    OutputOSS4::m_vc = this;
}

VolumeOSS4::~VolumeOSS4()
{
    VolumeSettings vol = volume();
    m_volume = (vol.right << 8) | vol.left;
    OutputOSS4::m_vc = nullptr;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("OSS4/volume", m_volume);
}

void VolumeOSS4::setVolume(const VolumeSettings &vol)
{
    m_volume = (vol.right << 8) | vol.left;
    if(OutputOSS4::instance() && OutputOSS4::instance()->fd() >= 0)
    {
        ioctl(OutputOSS4::instance()->fd(), SNDCTL_DSP_SETPLAYVOL, &m_volume);
    }
}

VolumeSettings VolumeOSS4::volume() const
{
    VolumeSettings vol;
    if(OutputOSS4::instance() && OutputOSS4::instance()->fd() >= 0)
    {
        int v = 0;
        if(ioctl(OutputOSS4::instance()->fd(), SNDCTL_DSP_GETPLAYVOL, &v) < 0)
            v = 0;
        vol.left = v & 0x00FF;
        vol.right = (v & 0xFF00) >> 8;
    }
    else
    {
        vol.left = m_volume & 0x00FF;
        vol.right = (m_volume & 0xFF00) >> 8;
    }
    return vol;
}

void VolumeOSS4::restore()
{
    if(OutputOSS4::instance() && OutputOSS4::instance()->fd() >= 0)
    {
        ioctl(OutputOSS4::instance()->fd(), SNDCTL_DSP_SETPLAYVOL, &m_volume);
    }
}
