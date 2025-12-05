#include "ksshelper.h"

#include <QSettings>

KSSHelper::KSSHelper(const QString &path)
    : m_path(path)
{

}

KSSHelper::~KSSHelper()
{
    deinit();
}

void KSSHelper::deinit()
{
    if(m_input)
    {
        KSSPLAY_delete(m_input);
    }

    if(m_kss)
    {
        KSS_delete(m_kss);
    }
}

bool KSSHelper::initialize()
{
    const QString &path = cleanPath();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("KSSHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_kss = KSS_bin2kss((uint8_t*)buffer.constData(), buffer.length(), QmmpPrintable(m_path));
    if(!m_kss)
    {
        qWarning("KSSHelper: KSS_bin2kss failed");
        return false;
    }

    m_input = KSSPLAY_new(sampleRate(), channels(), depth());
    if(!m_input)
    {
        qWarning("KSSHelper: KSSPLAY_new failed");
        return false;
    }

    KSSPLAY_set_data(m_input, m_kss);

    const int track = m_path.section("#", -1).toInt() - 1;
    KSSPLAY_reset(m_input, track < 0 ? 0 : track, 0);

    const uint32_t quality = 1;
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_PSG, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_SCC, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_OPL, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_OPLL, quality);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("KSS");
    const int master = settings.value("master_volume", 32).toInt();
    const int pan = settings.value("device_pan", 32).toInt();
    const int silence = settings.value("silence", 5).toInt();
    settings.endGroup();

    KSSPLAY_set_master_volume(m_input, master);

    KSSPLAY_set_device_pan(m_input, KSS_DEVICE_PSG, -pan);
    KSSPLAY_set_device_pan(m_input, KSS_DEVICE_SCC, pan);

    m_input->opll_stereo = 1;
    m_input->silent_limit = silence * 1000;

    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 0, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 1, 2);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 2, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 3, 2);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 4, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 5, 2);

    m_totalSamples = totalTime() * sampleRate() / 1000;
    return true;
}

void KSSHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        const int track = m_path.section("#", -1).toInt() - 1;
        KSSPLAY_reset(m_input, track < 0 ? 0 : track, 0);
        m_currentSample = 0;
    }

    if(m_currentSample != sample)
    {
        KSSPLAY_calc_silent(m_input, sample - m_currentSample);
        m_currentSample = sample;
    }
}

qint64 KSSHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    const int loop = KSSPLAY_get_loop_count(m_input);
    if(loop != m_loop || KSSPLAY_get_stop_flag(m_input))
    {
        m_loop = loop;
        return 0;
    }

    const int size = channels() * depth() / 8;
    m_currentSample += maxSize / size;
    KSSPLAY_calc(m_input, (int16_t*)data, maxSize / size);
    return maxSize;
}

QList<TrackInfo*> KSSHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    const int songs = m_kss->trk_max - m_kss->trk_min + 1;
    for(int i = 1; i <= songs; ++i)
    {
        KSSPLAY_reset(m_input, i - 1, 0);

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, KSS_get_title(m_kss));
            info->setValue(Qmmp::COMMENT, reinterpret_cast<char*>(m_kss->extra));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, reinterpret_cast<char*>(m_kss->idstr));
        }

        info->setPath("kss://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString KSSHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}
