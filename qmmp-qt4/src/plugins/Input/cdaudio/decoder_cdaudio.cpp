/***************************************************************************
 *   Copyright (C) 2009-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/


#include <QObject>
#include <QRegExp>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <cdio/cdio.h>
#if LIBCDIO_VERSION_NUM <= 83
#include <cdio/cdda.h>
#else
#include <cdio/paranoia/cdda.h>
#endif
#include <cdio/audio.h>
#include <cdio/cd_types.h>
#include <cdio/logging.h>
#include <cddb/cddb.h>
#include <qmmp/buffer.h>
#include <qmmp/output.h>
#include <qmmp/qmmpsettings.h>

#define CDDA_SECTORS 4
#define CDDA_BUFFER_SIZE (CDDA_SECTORS*CDIO_CD_FRAMESIZE_RAW)

#include "decoder_cdaudio.h"

QList <CDATrack> DecoderCDAudio::m_track_cache;

static void log_handler (cdio_log_level_t level, const char *message)
{
    QString str = QString::fromLocal8Bit(message).trimmed();
    switch (level)
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

static void cddb_log_handler(cddb_log_level_t level, const char *message)
{
    QString str = QString::fromLocal8Bit(message).trimmed();
    switch (level)
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

// Decoder class

DecoderCDAudio::DecoderCDAudio(const QString &url) : Decoder()
{
    m_bitrate = 0;
    m_totalTime = 0;
    m_first_sector = -1;
    m_last_sector  = -1;
    m_current_sector  = -1;
    m_url = url;
    m_cdio = 0;
    m_buffer_at = 0;
    m_buffer = new char[CDDA_BUFFER_SIZE];
}

DecoderCDAudio::~DecoderCDAudio()
{
    m_bitrate = 0;
    if (m_cdio)
    {
        cdio_destroy(m_cdio);
        m_cdio = 0;
    }
    delete [] m_buffer;
}

QList <CDATrack> DecoderCDAudio::generateTrackList(const QString &device)
{
    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    int cd_speed = settings.value("cdaudio/speed", 0).toInt();
    bool use_cd_text = settings.value("cdaudio/cdtext", true).toBool();
    QList <CDATrack> tracks;
    cdio_log_set_handler(log_handler); //setup cdio log handler
    CdIo_t *cdio = 0;
    QString device_path = device;
    if (device_path.isEmpty() || device_path == "/")
        device_path = settings.value("cdaudio/device").toString();
    if (device_path.isEmpty() || device_path == "/")
    {
        char **cd_drives = cdio_get_devices_with_cap(0, CDIO_FS_AUDIO, true); //get drive list with CDA disks
        // open first audio capable cd drive
        if (cd_drives && *cd_drives)
        {
            cdio = cdio_open_cd(*cd_drives);
            if (!cdio)
            {
                qWarning("DecoderCDAudio: failed to open CD.");
                cdio_free_device_list(cd_drives);
                return tracks;
            }
            qDebug("DecoderCDAudio: found cd audio capable drive \"%s\"", *cd_drives);
        }
        else
        {
            qWarning("DecoderCDAudio: unable to find cd audio drive.");
            cdio_free_device_list(cd_drives);
            return tracks;
        }
        device_path = QString(*cd_drives);
        if (cd_drives && *cd_drives) //free device list
            cdio_free_device_list(cd_drives);
    }
    else
    {
        cdio = cdio_open_cd(device_path.toAscii().constData());
        if (!cdio)
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

    if (cd_speed)
    {
        qDebug("DecoderCDAudio: setting drive speed to %dX.", cd_speed);
        if (cdio_set_speed(cdio, 1) != DRIVER_OP_SUCCESS)
            qWarning("DecoderCDAudio: unable to set drive speed to %dX.", cd_speed);
    }

    cdrom_drive_t *pcdrom_drive = cdio_cddap_identify_cdio(cdio, 1, 0); //create paranoya CD-ROM object
    //get first and last track numbers
    int first_track_number = cdio_get_first_track_num(pcdrom_drive->p_cdio);
    int last_track_number = cdio_get_last_track_num(pcdrom_drive->p_cdio);

    if ((first_track_number == CDIO_INVALID_TRACK) || (last_track_number == CDIO_INVALID_TRACK))
    {
        qWarning("DecoderCDAudio: invalid first (last) track number.");
        cdio_destroy(cdio);
        cdio = 0;
        return tracks;
    }
    bool use_cddb = true;
    //fill track list
    for (int i = first_track_number; i <= last_track_number; ++i)
    {
        CDATrack t;
        t.first_sector = cdio_get_track_lsn(pcdrom_drive->p_cdio, i);
        t.last_sector = cdio_get_track_last_lsn(pcdrom_drive->p_cdio, i);
        t.info.setLength((t.last_sector - t.first_sector +1) / 75);
        t.info.setMetaData(Qmmp::TRACK, i);
        t.info.setPath(QString("cdda://%1#%2").arg(device_path).arg(i));
        if ((t.first_sector == CDIO_INVALID_LSN) || (t.last_sector== CDIO_INVALID_LSN))
        {
            qWarning("DecoderCDAudio: invalid stard(end) lsn for the track %d.", i);
            tracks.clear();
            cdio_destroy(cdio);
            cdio = 0;
            return tracks;
        }
        //cd text
#if LIBCDIO_VERSION_NUM <= 83
        cdtext_t *cdtext = use_cd_text ? cdio_get_cdtext(pcdrom_drive->p_cdio, i) : 0;
        if (cdtext && cdtext->field[CDTEXT_TITLE])
        {
            t.info.setMetaData(Qmmp::TITLE, QString::fromLocal8Bit(cdtext->field[CDTEXT_TITLE]));
            t.info.setMetaData(Qmmp::ARTIST, QString::fromLocal8Bit(cdtext->field[CDTEXT_PERFORMER]));
            t.info.setMetaData(Qmmp::GENRE, QString::fromLocal8Bit(cdtext->field[CDTEXT_GENRE]));
            use_cddb = false;
        }
#else
        cdtext_t *cdtext = use_cd_text ? cdio_get_cdtext(pcdrom_drive->p_cdio) : 0;
        if (cdtext)
        {
            t.info.setMetaData(Qmmp::TITLE, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_TITLE,i)));
            t.info.setMetaData(Qmmp::ARTIST, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_PERFORMER,i)));
            t.info.setMetaData(Qmmp::GENRE, QString::fromUtf8(cdtext_get_const(cdtext,CDTEXT_FIELD_GENRE,i)));
            use_cddb = false;
        }
#endif
        else
            t.info.setMetaData(Qmmp::TITLE, QString("CDA Track %1").arg(i, 2, 10, QChar('0')));
        tracks  << t;
    }
    qDebug("DecoderCDAudio: found %d audio tracks", tracks.size());

    use_cddb = use_cddb && settings.value("cdaudio/use_cddb", false).toBool();
    if(use_cddb)
    {
        qDebug("DecoderCDAudio: reading CDDB...");
        cddb_log_set_handler(cddb_log_handler);
        cddb_conn_t *cddb_conn = cddb_new ();
        cddb_disc_t *cddb_disc = NULL;
        cddb_track_t *cddb_track = NULL;
        lba_t lba;
        if (!cddb_conn)
            qWarning ("DecoderCDAudio: unable to create cddb connection");
        else
        {
            cddb_cache_disable (cddb_conn); //disable libcddb cache, use own cache implementation instead
            settings.beginGroup("cdaudio");
            cddb_set_server_name (cddb_conn, settings.value("cddb_server", "freedb.org").toByteArray().constData());
            cddb_set_server_port (cddb_conn, settings.value("cddb_port", 8880).toInt());

            if (settings.value("cddb_http", false).toBool())
            {
                cddb_http_enable (cddb_conn);
                cddb_set_http_path_query (cddb_conn, settings.value("cddb_path").toByteArray());
                if (QmmpSettings::instance()->isProxyEnabled())
                {
                    QUrl proxy = QmmpSettings::instance()->proxy();
                    cddb_http_proxy_enable (cddb_conn);
                    cddb_set_http_proxy_server_name (cddb_conn, proxy.host().toAscii ());
                    cddb_set_http_proxy_server_port (cddb_conn, proxy.port());
                    if(QmmpSettings::instance()->useProxyAuth())
                    {
                        cddb_set_http_proxy_username (cddb_conn, proxy.userName().toAscii());
                        cddb_set_http_proxy_password (cddb_conn, proxy.password().toAscii());
                    }
                }
            }
            settings.endGroup();

            cddb_disc = cddb_disc_new ();
            lba = cdio_get_track_lba (cdio, CDIO_CDROM_LEADOUT_TRACK);
            cddb_disc_set_length (cddb_disc, FRAMES_TO_SECONDS (lba));

            for (int i = first_track_number; i <= last_track_number; ++i)
            {
                cddb_track = cddb_track_new ();
                cddb_track_set_frame_offset (cddb_track, cdio_get_track_lba (cdio, i));
                cddb_disc_add_track (cddb_disc, cddb_track);
            }

            cddb_disc_calc_discid (cddb_disc);
            uint id = cddb_disc_get_discid (cddb_disc);
            qDebug ("DecoderCDAudio: disc id = %x", id);


            if(readFromCache(&tracks, id))
                qDebug("DecoderCDAudio: using local cddb cache");
            else
            {
                int matches = cddb_query (cddb_conn, cddb_disc);
                if(matches == -1)
                {
                    qWarning ("DecoderCDAudio: unable to query the CDDB server, error: %s",
                              cddb_error_str (cddb_errno(cddb_conn)));
                }
                else if(matches == 0)
                {
                    qDebug ("DecoderCDAudio: no CDDB info found");
                }
                else if(cddb_read(cddb_conn, cddb_disc))
                {
                    for (int i = first_track_number; i <= last_track_number; ++i)
                    {
                        cddb_track_t *cddb_track = cddb_disc_get_track (cddb_disc, i - 1);
                        int t = i - first_track_number;
                        tracks[t].info.setMetaData(Qmmp::ARTIST,
                                                   QString::fromUtf8(cddb_track_get_artist(cddb_track)));
                        tracks[t].info.setMetaData(Qmmp::TITLE,
                                                   QString::fromUtf8(cddb_track_get_title(cddb_track)));
                        tracks[t].info.setMetaData(Qmmp::GENRE,
                                                   QString::fromUtf8(cddb_disc_get_genre(cddb_disc)));
                        tracks[t].info.setMetaData(Qmmp::ALBUM,
                                                   QString::fromUtf8(cddb_disc_get_title(cddb_disc)));
                    }
                    saveToCache(tracks,  id);
                }
                else
                {
                    qWarning ("DecoderCDAudio: unable to read the CDDB info: %s",
                              cddb_error_str (cddb_errno(cddb_conn)));
                }
            }
        }
        if (cddb_disc)
            cddb_disc_destroy (cddb_disc);

        if (cddb_conn)
            cddb_destroy (cddb_conn);
    }

    cdio_destroy(cdio);
    cdio = 0;
    m_track_cache = tracks;
    return tracks;
}

void DecoderCDAudio::saveToCache(QList <CDATrack> tracks,  uint disc_id)
{
    QDir dir(Qmmp::configDir());
    if(!dir.exists("cddbcache"))
        dir.mkdir("cddbcache");
    dir.cd("cddbcache");
    QString path = dir.absolutePath() + QString("/%1").arg(disc_id, 0, 16);
    QSettings settings(path, QSettings::IniFormat);
    settings.clear();
    settings.setValue("count", tracks.size());
    for(int i = 0; i < tracks.size(); ++i)
    {
        CDATrack track = tracks[i];
        QMap<Qmmp::MetaData, QString> meta = track.info.metaData();
        settings.setValue(QString("artist%1").arg(i), meta[Qmmp::ARTIST]);
        settings.setValue(QString("title%1").arg(i), meta[Qmmp::TITLE]);
        settings.setValue(QString("genre%1").arg(i), meta[Qmmp::GENRE]);
        settings.setValue(QString("album%1").arg(i), meta[Qmmp::ALBUM]);
    }
}

bool DecoderCDAudio::readFromCache(QList <CDATrack> *tracks, uint disc_id)
{
    QString path = Qmmp::configDir();
    path += QString("/cddbcache/%1").arg(disc_id, 0, 16);
    if(!QFile::exists(path))
        return false;
    QSettings settings(path, QSettings::IniFormat);
    int count = settings.value("count").toInt();
    if(count != tracks->count())
        return false;
    for(int i = 0; i < count; ++i)
    {
        (*tracks)[i].info.setMetaData(Qmmp::ARTIST, settings.value(QString("artist%1").arg(i)).toString());
        (*tracks)[i].info.setMetaData(Qmmp::TITLE, settings.value(QString("title%1").arg(i)).toString());
        (*tracks)[i].info.setMetaData(Qmmp::GENRE, settings.value(QString("genre%1").arg(i)).toString());
        (*tracks)[i].info.setMetaData(Qmmp::ALBUM, settings.value(QString("album%1").arg(i)).toString());
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
    //extract track from url
    int track_number = m_url.section("#", -1).toInt();
    QString device_path = m_url;
    device_path.remove("cdda://");
    device_path.remove(QRegExp("#\\d+$"));

    track_number = qMax(track_number, 1);
    QList <CDATrack> tracks = DecoderCDAudio::generateTrackList(device_path); //generate track list
    if (tracks.isEmpty())
    {
        qWarning("DecoderCDAudio: initialize failed");
        return false;
    }
    //find track by number
    int track_at = -1;
    for (int i = 0; i < tracks.size(); ++i)
        if (tracks[i].info.metaData(Qmmp::TRACK).toInt() == track_number)
        {
            track_at = i;
            break;
        }
    if (track_at < 0)
    {
        qWarning("DecoderCDAudio: invalid track number");
        return false;
    }

    if (device_path.isEmpty() || device_path == "/") //try default path from config
    {
        QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
        device_path = settings.value("cdaudio/device").toString();
        m_url = QString("cdda://%1#%2").arg(device_path).arg(track_number);
    }

    if (device_path.isEmpty() || device_path == "/")
    {
        char **cd_drives = cdio_get_devices_with_cap(0, CDIO_FS_AUDIO, true); //get drive list with CDA disks
        // open first audio capable cd drive
        if (cd_drives && *cd_drives)
        {
            m_cdio = cdio_open_cd(*cd_drives);
            if (!m_cdio)
            {
                qWarning("DecoderCDAudio: failed to open CD.");
                cdio_free_device_list(cd_drives);
                return false;
            }
            qDebug("DecoderCDAudio: found cd audio capable drive \"%s\"", *cd_drives);
        }
        else
        {
            qWarning("DecoderCDAudio: unable to find cd audio drive.");
            return false;
        }
        if (cd_drives && *cd_drives) //free device list
            cdio_free_device_list(cd_drives);
    }
    else
    {
        m_cdio = cdio_open_cd(device_path.toAscii().constData());
        if (!m_cdio)
        {
            qWarning("DecoderCDAudio: failed to open CD.");
            return false;
        }
        qDebug("DecoderCDAudio: using cd audio capable drive \"%s\"", qPrintable(device_path));
    }
    configure(44100, 2, Qmmp::PCM_S16LE);
    m_bitrate = 1411;
    m_totalTime = tracks[track_at].info.length() * 1000;
    m_first_sector = tracks[track_at].first_sector;
    m_current_sector = tracks[track_at].first_sector;
    m_last_sector = tracks[track_at].last_sector;
    addMetaData(tracks[track_at].info.metaData()); //send metadata
    qDebug("DecoderCDAudio: initialize succes");
    return true;
}


qint64 DecoderCDAudio::totalTime()
{
    return m_totalTime;
}

int DecoderCDAudio::bitrate()
{
    return m_bitrate;
}

qint64 DecoderCDAudio::read(unsigned char *audio, qint64 maxSize)
{
    if(!m_buffer_at)
    {

        lsn_t secorts_to_read = qMin(CDDA_SECTORS, (m_last_sector - m_current_sector + 1));

        if (secorts_to_read <= 0)
            return 0;

        if (cdio_read_audio_sectors(m_cdio, m_buffer,
                                    m_current_sector, secorts_to_read) != DRIVER_OP_SUCCESS)
        {
            m_buffer_at = 0;
            return -1;
        }
        else
        {
            m_buffer_at = secorts_to_read * CDIO_CD_FRAMESIZE_RAW;
            m_current_sector += secorts_to_read;
        }
    }

    if(m_buffer_at > 0)
    {
        long len = qMin(maxSize, m_buffer_at);
        memcpy(audio, m_buffer, len);
        m_buffer_at -= len;
        memmove(m_buffer, m_buffer + len, m_buffer_at);
        return len;
    }
    return 0;
}

void DecoderCDAudio::seek(qint64 pos)
{
    m_current_sector = m_first_sector + pos * 75 / 1000;
    m_buffer_at = 0;
}
