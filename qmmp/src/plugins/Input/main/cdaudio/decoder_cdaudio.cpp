#include "decoder_cdaudio.h"

#include <QDir>
#include <QSettings>
#include <cdio/paranoia/cdda.h>
#include <cdio/audio.h>
#include <cdio/cd_types.h>
#include <cdio/logging.h>
#ifdef WITH_LIBCDDB
#include <cddb/cddb.h>
#endif
#include <qmmp/qmmpsettings.h>

#define CDDA_SECTORS 4
#define CDDA_BUFFER_SIZE (CDDA_SECTORS*CDIO_CD_FRAMESIZE_RAW)


QList<CDATrack> DecoderCDAudio::m_track_cache;

static void log_handler(cdio_log_level_t level, const char *message)
{
    QString str = QString::fromLocal8Bit(message).trimmed();
    switch(level)
    {
    case CDIO_LOG_DEBUG:
        qDebug("DecoderCDAudio: cdio message: %s (level=debug)", qPrintable(str));
        return;
    case CDIO_LOG_INFO:
        qDebug("DecoderCDAudio: cdio message: %s (level=info)", qPrintable(str));
        return;
    default:
        qWarning("DecoderCDAudio: cdio message: %s (level=error)", qPrintable(str));
    }
}

#ifdef WITH_LIBCDDB
static void cddb_log_handler(cddb_log_level_t level, const char *message)
{
    QString str = QString::fromLocal8Bit(message).trimmed();
    switch(level)
    {
    case CDDB_LOG_DEBUG:
        qDebug("DecoderCDAudio: cddb message: %s (level=debug)", qPrintable(str));
        return;
    case CDDB_LOG_INFO:
        qDebug("DecoderCDAudio: cddb message: %s (level=info)", qPrintable(str));
        return;
    default:
        qWarning("DecoderCDAudio: cddb message: %s (level=error)", qPrintable(str));
    }
}
#endif


DecoderCDAudio::DecoderCDAudio(const QString &path)
    : Decoder(),
      m_path(path),
      m_buffer(new char[CDDA_BUFFER_SIZE])
{

}

DecoderCDAudio::~DecoderCDAudio()
{
    m_bitrate = 0;
    if(m_cdio)
    {
        cdio_destroy(m_cdio);
        m_cdio = nullptr;
    }
    delete[] m_buffer;
}

QList<CDATrack> DecoderCDAudio::generateTrackList(const QString &path, TrackInfo::Parts parts)
{
    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const int cd_speed = settings.value("CDAudio/speed", 0).toInt();
    const bool use_cd_text = settings.value("CDAudio/cdtext", true).toBool();

    QList<CDATrack> tracks;
    cdio_log_set_handler(log_handler); //setup cdio log handler
    CdIo_t *cdio = nullptr;
    QString device_path = path;
    if(device_path.isEmpty() || device_path == "/")
        device_path = settings.value("CDAudio/device").toString();
    if(device_path.isEmpty() || device_path == "/")
    {
        char **cd_drives = cdio_get_devices_with_cap(nullptr, CDIO_FS_AUDIO, true); //get drive list with CDA disks
        // open first audio capable cd drive
        if(cd_drives && *cd_drives)
        {
            cdio = cdio_open_cd(*cd_drives);
            if(!cdio)
            {
                qWarning("DecoderCDAudio: failed to open CD.");
                cdio_free_device_list(cd_drives);
                return tracks;
            }
            qDebug("DecoderCDAudio: found cd audio capable drive \"%s\"", *cd_drives);
            device_path = QString(*cd_drives);
            cdio_free_device_list(cd_drives); //free device list
        }
        else
        {
            qWarning("DecoderCDAudio: unable to find cd audio drive.");
            cdio_free_device_list(cd_drives);
            return tracks;
        }
    }
    else
    {
        cdio = cdio_open_cd(device_path.toLatin1().constData());
        if(!cdio)
        {
            qWarning("DecoderCDAudio: failed to open CD.");
            return tracks;
        }
        qDebug("DecoderCDAudio: using cd audio capable drive \"%s\"", qPrintable(device_path));
    }

    if(!m_track_cache.isEmpty() && !cdio_get_media_changed(cdio))
    {
        qDebug("DecoderCDAudio: using track cache...");
        cdio_destroy(cdio);
        return m_track_cache;
    }

    if(cd_speed)
    {
        qDebug("DecoderCDAudio: setting drive speed to %dX.", cd_speed);
        if(cdio_set_speed(cdio, 1) != DRIVER_OP_SUCCESS)
            qWarning("DecoderCDAudio: unable to set drive speed to %dX.", cd_speed);
    }

    cdrom_drive_t *pcdrom_drive = cdio_cddap_identify_cdio(cdio, 1, nullptr); //create paranoya CD-ROM object
    //get first and last track numbers
    int first_track_number = cdio_get_first_track_num(pcdrom_drive->p_cdio);
    int last_track_number = cdio_get_last_track_num(pcdrom_drive->p_cdio);

    if((first_track_number == CDIO_INVALID_TRACK) || (last_track_number == CDIO_INVALID_TRACK))
    {
        qWarning("DecoderCDAudio: invalid first (last) track number.");
        cdio_destroy(cdio);
        cdio = nullptr;
        return tracks;
    }
#ifdef WITH_LIBCDDB
    bool use_cddb = true;
#endif
    //fill track list
    for(int i = first_track_number; i <= last_track_number; ++i)
    {
        CDATrack t;
        t.first_sector = cdio_get_track_lsn(pcdrom_drive->p_cdio, i);
        t.last_sector = cdio_get_track_last_lsn(pcdrom_drive->p_cdio, i);
        t.info.setDuration((t.last_sector - t.first_sector +1) * 1000 / 75);
        t.info.setValue(Qmmp::TRACK, i);
        t.info.setPath(QString("cdda://%1#%2").arg(device_path).arg(i));

        if(parts & TrackInfo::Properties)
        {
            t.info.setValue(Qmmp::BITRATE, 1411);
            t.info.setValue(Qmmp::SAMPLERATE, 44100);
            t.info.setValue(Qmmp::CHANNELS, 2);
            t.info.setValue(Qmmp::BITS_PER_SAMPLE, 16);
            t.info.setValue(Qmmp::FORMAT_NAME, "CDDA");
        }

        if((t.first_sector == CDIO_INVALID_LSN) || (t.last_sector== CDIO_INVALID_LSN))
        {
            qWarning("DecoderCDAudio: invalid stard(end) lsn for the track %d.", i);
            tracks.clear();
            cdio_destroy(cdio);
            cdio = nullptr;
            return tracks;
        }
        //cd text
        cdtext_t *cdtext = use_cd_text ? cdio_get_cdtext(pcdrom_drive->p_cdio) : nullptr;
        if(cdtext)
        {
            t.info.setValue(Qmmp::TITLE, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_TITLE,i)));
            t.info.setValue(Qmmp::ARTIST, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_PERFORMER,i)));
            t.info.setValue(Qmmp::GENRE, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_GENRE,i)));
            t.info.setValue(Qmmp::COMMENT, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_MESSAGE,i)));
            t.info.setValue(Qmmp::COMPOSER, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_COMPOSER,i)));
#ifdef WITH_LIBCDDB
            use_cddb = false;
#endif
        }
        else
            t.info.setValue(Qmmp::TITLE, QString("CDA Track %1").arg(i, 2, 10, QChar('0')));
        tracks  << t;
    }
    qDebug("DecoderCDAudio: found %d audio tracks", tracks.count());

#ifdef WITH_LIBCDDB
    use_cddb = use_cddb && settings.value("CDAudio/use_cddb", false).toBool();
    if(use_cddb)
    {
        qDebug("DecoderCDAudio: reading CDDB...");
        cddb_log_set_handler(cddb_log_handler);
        cddb_conn_t *cddb_conn = cddb_new();
        cddb_disc_t *cddb_disc = nullptr;
        lba_t lba;
        if(!cddb_conn)
            qWarning("DecoderCDAudio: unable to create cddb connection");
        else
        {
            cddb_cache_disable(cddb_conn); //disable libcddb cache, use own cache implementation instead
            settings.beginGroup("CDAudio");
            cddb_set_server_name(cddb_conn, settings.value("cddb_server", "gnudb.org").toByteArray().constData());
            cddb_set_server_port(cddb_conn, settings.value("cddb_port", 8880).toInt());

            if(settings.value("cddb_http", false).toBool())
            {
                cddb_http_enable(cddb_conn);
                cddb_set_http_path_query(cddb_conn, settings.value("cddb_path").toByteArray().constData());
                if(QmmpSettings::instance()->isProxyEnabled() && QmmpSettings::instance()->proxyType() == QmmpSettings::HTTP_PROXY)
                {
                    QUrl proxy = QmmpSettings::instance()->proxy();
                    cddb_http_proxy_enable(cddb_conn);
                    cddb_set_http_proxy_server_name(cddb_conn, proxy.host().toLatin1().constData());
                    cddb_set_http_proxy_server_port(cddb_conn, proxy.port());
                    if(QmmpSettings::instance()->useProxyAuth())
                    {
                        cddb_set_http_proxy_username(cddb_conn, proxy.userName().toLatin1().constData());
                        cddb_set_http_proxy_password(cddb_conn, proxy.password().toLatin1().constData());
                    }
                }
            }
            settings.endGroup();

            cddb_disc = cddb_disc_new();
            lba = cdio_get_track_lba(cdio, CDIO_CDROM_LEADOUT_TRACK);
            cddb_disc_set_length(cddb_disc, FRAMES_TO_SECONDS(lba));

            for(int i = first_track_number; i <= last_track_number; ++i)
            {
                cddb_track_t *cddb_track = cddb_track_new();
                cddb_track_set_frame_offset(cddb_track, cdio_get_track_lba(cdio, i));
                cddb_disc_add_track(cddb_disc, cddb_track);
            }

            cddb_disc_calc_discid(cddb_disc);
            uint id = cddb_disc_get_discid(cddb_disc);
            qDebug("DecoderCDAudio: disc id = %x", id);


            if(readFromCache(&tracks, id))
                qDebug("DecoderCDAudio: using local cddb cache");
            else
            {
                int matches = cddb_query(cddb_conn, cddb_disc);
                if(matches == -1)
                {
                    qWarning("DecoderCDAudio: unable to query the CDDB server, error: %s",
                              cddb_error_str(cddb_errno(cddb_conn)));
                }
                else if(matches == 0)
                {
                    qDebug("DecoderCDAudio: no CDDB info found");
                }
                else if(cddb_read(cddb_conn, cddb_disc))
                {
                    for(int i = first_track_number; i <= last_track_number; ++i)
                    {
                        cddb_track_t *cddb_track = cddb_disc_get_track(cddb_disc, i - 1);
                        int t = i - first_track_number;
                        tracks[t].info.setValue(Qmmp::ARTIST, QString::fromUtf8(cddb_track_get_artist(cddb_track)));
                        tracks[t].info.setValue(Qmmp::TITLE, QString::fromUtf8(cddb_track_get_title(cddb_track)));
                        tracks[t].info.setValue(Qmmp::GENRE, QString::fromUtf8(cddb_disc_get_genre(cddb_disc)));
                        tracks[t].info.setValue(Qmmp::ALBUM, QString::fromUtf8(cddb_disc_get_title(cddb_disc)));
                        tracks[t].info.setValue(Qmmp::YEAR, cddb_disc_get_year(cddb_disc));
                    }
                    saveToCache(tracks,  id);
                }
                else
                {
                    qWarning("DecoderCDAudio: unable to read the CDDB info: %s",
                              cddb_error_str(cddb_errno(cddb_conn)));
                }
            }
        }
        if(cddb_disc)
            cddb_disc_destroy(cddb_disc);

        if(cddb_conn)
            cddb_destroy(cddb_conn);
    }
#endif

    cdio_destroy(cdio);
    cdio = nullptr;
    m_track_cache = tracks;
    return tracks;
}

void DecoderCDAudio::saveToCache(const QList<CDATrack> &tracks,  uint disc_id)
{
    QDir dir(Qmmp::configDir());
    if(!dir.exists("cddbcache"))
        dir.mkdir("cddbcache");
    dir.cd("cddbcache");
    QString path = dir.absolutePath() + QString("/%1").arg(disc_id, 0, 16);
    QSettings settings(path, QSettings::IniFormat);
    settings.clear();
    settings.setValue("count", tracks.count());
    for(int i = 0; i < tracks.count(); ++i)
    {
        CDATrack track = tracks[i];
        QMap<Qmmp::MetaData, QString> meta = track.info.metaData();
        settings.setValue(QString("artist%1").arg(i), meta[Qmmp::ARTIST]);
        settings.setValue(QString("title%1").arg(i), meta[Qmmp::TITLE]);
        settings.setValue(QString("genre%1").arg(i), meta[Qmmp::GENRE]);
        settings.setValue(QString("album%1").arg(i), meta[Qmmp::ALBUM]);
        settings.setValue(QString("year%1").arg(i), meta[Qmmp::YEAR]);
    }
}

bool DecoderCDAudio::readFromCache(QList<CDATrack> *tracks, uint disc_id)
{
    QString path = Qmmp::cacheDir();
    path += QString("/cddbcache/%1").arg(disc_id, 0, 16);
    if(!QFile::exists(path))
        return false;
    const QSettings settings(path, QSettings::IniFormat);
    int count = settings.value("count").toInt();
    if(count != tracks->count())
        return false;
    for(int i = 0; i < count; ++i)
    {
        (*tracks)[i].info.setValue(Qmmp::ARTIST, settings.value(QString("artist%1").arg(i)).toString());
        (*tracks)[i].info.setValue(Qmmp::TITLE, settings.value(QString("title%1").arg(i)).toString());
        (*tracks)[i].info.setValue(Qmmp::GENRE, settings.value(QString("genre%1").arg(i)).toString());
        (*tracks)[i].info.setValue(Qmmp::ALBUM, settings.value(QString("album%1").arg(i)).toString());
        (*tracks)[i].info.setValue(Qmmp::YEAR, settings.value(QString("year%1").arg(i)).toString());
    }
    return true;
}

qint64 DecoderCDAudio::calculateTrackLength(lsn_t startlsn, lsn_t endlsn)
{
    return ((endlsn - startlsn + 1) * 1000) / 75;
}

void DecoderCDAudio::clearTrackCache()
{
    m_track_cache.clear();
}

bool DecoderCDAudio::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;
    //extract track from path
    int track = -1;
    QString device_path = TrackInfo::pathFromUrl(m_path, &track);

    track = qMax(track, 1);
    QList<CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path); //generate track list
    if(tracks.isEmpty())
    {
        qWarning("DecoderCDAudio: initialize failed");
        return false;
    }
    //find track by number
    int track_at = -1;
    for(int i = 0; i < tracks.count(); ++i)
        if(tracks[i].info.value(Qmmp::TRACK).toInt() == track)
        {
            track_at = i;
            break;
        }
    if(track_at < 0)
    {
        qWarning("DecoderCDAudio: invalid track number");
        return false;
    }

    if(device_path.isEmpty() || device_path == "/") //try default path from config
    {
        const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        device_path = settings.value("CDAudio/device").toString();
        m_path = QString("cdda://%1#%2").arg(device_path).arg(track);
    }

    if(device_path.isEmpty() || device_path == "/")
    {
        char **cd_drives = cdio_get_devices_with_cap(nullptr, CDIO_FS_AUDIO, true); //get drive list with CDA disks
        // open first audio capable cd drive
        if(cd_drives && *cd_drives)
        {
            m_cdio = cdio_open_cd(*cd_drives);
            if(!m_cdio)
            {
                qWarning("DecoderCDAudio: failed to open CD.");
                cdio_free_device_list(cd_drives);
                return false;
            }
            qDebug("DecoderCDAudio: found cd audio capable drive \"%s\"", *cd_drives);
            cdio_free_device_list(cd_drives);  //free device list
        }
        else
        {
            qWarning("DecoderCDAudio: unable to find cd audio drive.");
            return false;
        }
    }
    else
    {
        m_cdio = cdio_open_cd(device_path.toLatin1().constData());
        if(!m_cdio)
        {
            qWarning("DecoderCDAudio: failed to open CD.");
            return false;
        }
        qDebug("DecoderCDAudio: using cd audio capable drive \"%s\"", qPrintable(device_path));
    }

    configure(44100, 2, Qmmp::PCM_S16LE);
    m_bitrate = 1411;
    m_totalTime = tracks[track_at].info.duration();
    m_first_sector = tracks[track_at].first_sector;
    m_current_sector = tracks[track_at].first_sector;
    m_last_sector = tracks[track_at].last_sector;

    addMetaData(tracks[track_at].info.metaData()); //send metadata
    setProperty(Qmmp::FORMAT_NAME, "CDDA");
    setProperty(Qmmp::BITRATE, m_bitrate);
    qDebug("DecoderCDAudio: initialize success");
    return true;
}

qint64 DecoderCDAudio::totalTime() const
{
    return m_totalTime;
}

int DecoderCDAudio::bitrate() const
{
    return m_bitrate;
}

qint64 DecoderCDAudio::read(unsigned char *data, qint64 maxSize)
{
    if(!m_buffer_at)
    {
        lsn_t secorts_to_read = qMin(CDDA_SECTORS, (m_last_sector - m_current_sector + 1));

        if(secorts_to_read <= 0)
            return 0;

        if(cdio_read_audio_sectors(m_cdio, m_buffer,
                                    m_current_sector, secorts_to_read) != DRIVER_OP_SUCCESS)
        {
            m_buffer_at = 0;
            return -1;
        }

        m_buffer_at = secorts_to_read * CDIO_CD_FRAMESIZE_RAW;
        m_current_sector += secorts_to_read;
    }

    if(m_buffer_at > 0)
    {
        long len = qMin(maxSize, m_buffer_at);
        memcpy(data, m_buffer, len);
        m_buffer_at -= len;
        memmove(m_buffer, m_buffer + len, m_buffer_at);
        return len;
    }
    return 0;
}

void DecoderCDAudio::seek(qint64 time)
{
    m_current_sector = m_first_sector + time * 75 / 1000;
    m_buffer_at = 0;
}
