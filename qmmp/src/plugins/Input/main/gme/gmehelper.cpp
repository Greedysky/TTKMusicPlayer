#include "gmehelper.h"

#include <QSettings>

GMEHelper::GMEHelper()
{
     QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
     m_fade_length = settings.value("GME/fadeout_length", 7000).toInt();
     if(settings.value("GME/fadeout", false).toBool())
         m_fade_length = 0;
}

GMEHelper::~GMEHelper()
{
    if(m_emu)
        gme_delete(m_emu);
    m_emu = nullptr;
}

Music_Emu *GMEHelper::load(const QString &path, int sample_rate)
{
    if(m_emu)
        gme_delete(m_emu);
    m_emu = nullptr;

    m_path = path.contains("://") ? TrackInfo::pathFromUrl(path) : path;

    const char *err = nullptr;
    gme_type_t file_type;
    if((err = gme_identify_file(QmmpPrintable(m_path), &file_type)))
    {
        qWarning("GMEHelper: %s", err);
        return nullptr;
    }

    if(!file_type)
    {
        qWarning("DecoderGme: unsupported music type");
        return nullptr;
    }

    if(!(m_emu = gme_new_emu(file_type, sample_rate)))
    {
        qWarning("GMEHelper: out of memory");
        return nullptr;
    }

    if((err = gme_load_file(m_emu, QmmpPrintable(m_path))))
    {
        qWarning("GMEHelper: %s", err);
        return nullptr;
    }

    QString m3u_path = m_path.left(m_path.lastIndexOf("."));
    m3u_path.append(".m3u");
    gme_load_m3u(m_emu, QmmpPrintable(m3u_path));
    return m_emu;
}

QList<TrackInfo*> GMEHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_emu)
    {
        return playlist;
    }

    const int count = gme_track_count(m_emu);
    gme_info_t *track_info;

    for(int i = 1; i <= count; ++i)
    {
        TrackInfo *info = new TrackInfo();
        if(!gme_track_info(m_emu, &track_info, i-1))
        {
            if(track_info->length <= 0)
                track_info->length = track_info->intro_length + track_info->loop_length * 2;
        }

        if(track_info->length <= 0)
            track_info->length = (long) (2.5 * 60 * 1000);

        if(track_info->length < m_fade_length)
            track_info->length += m_fade_length;

        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::ALBUM, track_info->game);
            info->setValue(Qmmp::TITLE, track_info->song);
            info->setValue(Qmmp::ARTIST, track_info->author);
            info->setValue(Qmmp::COMMENT, track_info->comment);
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, 8);
            info->setValue(Qmmp::SAMPLERATE, 44100);
            info->setValue(Qmmp::CHANNELS, 2);
            info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
            info->setValue(Qmmp::FORMAT_NAME, track_info->system);
        }

        info->setPath("gme://" + m_path + QString("#%1").arg(i));
        info->setDuration(track_info->length);
        gme_free_info(track_info);
        playlist << info;
    }
    return playlist;
}

int GMEHelper::fadeLength()
{
    return m_fade_length;
}
