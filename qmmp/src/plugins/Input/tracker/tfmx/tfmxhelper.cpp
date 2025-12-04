#include "tfmxhelper.h"

#include <QSettings>

TFMXHelper::TFMXHelper(const QString &path)
    : m_path(path),
      m_sampleRate(44100)
{

}

TFMXHelper::~TFMXHelper()
{
    deinit();
}

void TFMXHelper::deinit()
{
    if(m_input)
    {
        tfmxdec_delete(m_input);
    }
}

bool TFMXHelper::initialize()
{
    const QString &path = cleanPath();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("TFMXHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = tfmxdec_new();

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("TFMX");
    switch(settings.value("sample_rate", 1).toInt())
    {
        case 0: m_sampleRate = 22050; break;
        case 1: m_sampleRate = 44100; break;
        case 2: m_sampleRate = 48000; break;
        default: m_sampleRate = 44100; break;
    }
    const int panning = settings.value("panning", 75).toInt();
    const int secs = settings.value("min_duration", 10).toInt();
    const bool flag = settings.value("end_shorts", true).toBool();
    settings.endGroup();

    tfmxdec_set_path(m_input, QmmpPrintable(path));
    tfmxdec_end_shorts(m_input, flag ? 1 : 0, secs);
    tfmxdec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000, panning);

    const int track = m_path.section("#", -1).toInt() - 1;
    if(!tfmxdec_init(m_input, (void*)buffer.constData(), buffer.length(), track < 0 ? 0 : track))
    {
        qWarning("TFMXHelper: tfmxdec_init error");
        return false;
    }
    return true;
}

qint64 TFMXHelper::read(unsigned char *data, qint64 maxSize)
{
    tfmxdec_buffer_fill(m_input, data, maxSize);
    return tfmxdec_song_end(m_input) ? 0 : maxSize;
}

QList<TrackInfo*> TFMXHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    QFileInfo fin(cleanPath());
    QString title = fin.baseName();
    if(!filters().contains("*." + fin.suffix(), Qt::CaseInsensitive))
    {
        title = fin.suffix();
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("TFMX");
    const int secs = settings.value("min_duration", 10).toInt();
    settings.endGroup();

    const int songs = tfmxdec_songs(m_input);
    for(int i = 1; i <= songs; ++i)
    {
        tfmxdec_reinit(m_input, i - 1);
        const int duration = totalTime();
        if(songs > 1 && (duration / 1000) < secs)
        {
            continue;
        }

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            const char *v = tfmxdec_get_title(m_input);
            if(v && strlen(v) > 0)
            {
                info->setValue(Qmmp::TITLE, v);
            }
            else
            {
                info->setValue(Qmmp::TITLE, title);
            }

            info->setValue(Qmmp::ARTIST, tfmxdec_get_artist(m_input));
            info->setValue(Qmmp::ALBUM, tfmxdec_get_game(m_input));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, tfmxdec_format_name(m_input));
        }

        info->setPath("tfmx://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(duration);
        playlist << info;
    }
    return playlist;
}

QString TFMXHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}

QStringList TFMXHelper::filters()
{
    const QStringList filters =
    {
        "*.tfm",
        "*.tfmx",
        "*.fc", "*.fc13", "*.fc14", "*.fc3", "*.fc4", ".smod",
        "*.hip", "*.hipc", "*.hip7", "*.mcmd",
        // pair suffix section
        "*.mdat", // (mdat, smpl))
        "*.tfx",  // (tfx, (smp, sam))
        // pair prefix section
        "mdat.*"  // (mdat, smpl)
    };
    return filters;
}
