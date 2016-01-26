/***************************************************************************
 *   Copyright (C) 2013 by Ilya Kotov                                      *
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

#include <QFileInfo>
#include <QSettings>
#include <sidplayfp/sidplayfp.h>
#include <sidplayfp/SidTune.h>
#include <sidplayfp/sidbuilder.h>
#include <sidplayfp/SidConfig.h>
#include <sidplayfp/builders/residfp.h>
#include <sidplayfp/builders/resid.h>
#include <sidplayfp/SidInfo.h>
#include <sidplayfp/SidTuneInfo.h>
#include <sidplayfp/SidDatabase.h>
#include "decoder_sid.h"

// Decoder class
DecoderSID::DecoderSID(SidDatabase *db, const QString &url) : Decoder()
{
    m_db = db;
    m_url = url;
    m_player = new sidplayfp();
    m_length = 0;
    m_length_in_bytes = 0;
    m_read_bytes = 0;
}

DecoderSID::~DecoderSID()
{
    delete m_player;
}

bool DecoderSID::initialize()
{
    m_length_in_bytes = 0;
    m_read_bytes = 0;
    QString path = m_url;
    path.remove("sid://");
    path.remove(QRegExp("#\\d+$"));
    int track = m_url.section("#", -1).toInt();

    SidTune *tune = new SidTune(0);
    tune->load(qPrintable(path));
    if(!tune->getInfo())
    {
        qWarning("DecoderSID: unable to load tune, error: %s", tune->statusString());
        delete tune;
        return false;
    }
    int count = tune->getInfo()->songs();

    if(track > count || track < 1)
    {
        qWarning("DecoderSID: track number is out of range");
        delete tune;
        return false;
    }

    tune->selectSong(track);

    if(!tune->getStatus())
    {
        qWarning("DecoderSID: error: %s", tune->statusString());
        return false;
    }

    //send metadata for pseudo-protocol
    const SidTuneInfo *tune_info = tune->getInfo();
    QMap<Qmmp::MetaData, QString> metadata;
    metadata.insert(Qmmp::TITLE,  tune_info->infoString(0));
    metadata.insert(Qmmp::ARTIST, tune_info->infoString(1));
    metadata.insert(Qmmp::COMMENT, tune_info->commentString(0));
    metadata.insert(Qmmp::TRACK, QString("%1").arg(track));
    metadata.insert(Qmmp::URL, m_url);
    addMetaData(metadata);

    //read settings
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("SID");
    if(settings.value("use_hvsc", false).toBool())
    {
        char md5[SidTune::MD5_LENGTH+1];
        tune->createMD5(md5);
        m_length = m_db->length(md5, track);
    }

    if(m_length <= 0)
        m_length = settings.value("song_length", 180).toInt();

    qDebug("DecoderSID: song length: %d", m_length);

    sidbuilder *rs = 0;
    if(settings.value("engine", "residfp").toString() == "residfp")
    {
        rs = new ReSIDfpBuilder("ReSIDfp builder");
        qDebug("DecoderSID: using ReSIDfp emulation");
    }
    else
    {
        rs = new ReSIDBuilder("ReSID builder");
        qDebug("DecoderSID: using ReSID emulation");
    }
    rs->create(m_player->info().maxsids());

    SidConfig cfg = m_player->config();
    cfg.frequency    = settings.value("sample_rate", 44100).toInt();
    int sm = settings.value("resampling_method", SidConfig::INTERPOLATE).toInt();
    cfg.samplingMethod = (SidConfig::sampling_method_t) sm;
    cfg.playback     = SidConfig::STEREO;
    cfg.sidEmulation = rs;
    cfg.fastSampling = settings.value("fast_resampling", false).toBool();
    settings.endGroup();

    if(!m_player->config(cfg))
    {
        qWarning("DecoderSID: unable to load config, error: %s", m_player->error());
        return false;
    }

    if(!m_player->load(tune))
    {
        qWarning("DecoderSID: unable to load tune, error: %s", m_player->error());
        return false;
    }

    configure(44100, 2);
    m_length_in_bytes = audioParameters().sampleRate() *
            audioParameters().channels() *
            audioParameters().sampleSize() * m_length;
    qDebug("DecoderSID: initialize succes");
    return true;
}

qint64 DecoderSID::totalTime()
{
    return 0;
}

void DecoderSID::seek(qint64 pos)
{
    Q_UNUSED(pos);
}

int DecoderSID::bitrate()
{
    return 8;
}

qint64 DecoderSID::read(char *data, qint64 size)
{
    size = qMin(size, qMax(m_length_in_bytes - m_read_bytes, qint64(0)));
    size -= size % 4;
    if(size <= 0)
        return 0;
    m_read_bytes += size;
    return m_player->play((short *)data,     size/2) * 2;
}
