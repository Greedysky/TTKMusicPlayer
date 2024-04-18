/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#ifndef DECODER_CDAUIDO_H
#define DECODER_CDAUIDO_H

#include <cdio/cdio.h>
#include <qmmp/decoder.h>
#include <qmmp/trackinfo.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class CDATrack
{
public:
    CDATrack()
    {

    }

    TrackInfo info;
    lsn_t first_sector = 0;
    lsn_t last_sector = 0;
};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderCDAudio : public Decoder
{
public:
    explicit DecoderCDAudio(const QString &path);
    virtual ~DecoderCDAudio();

    static QList<CDATrack> generateTrackList(const QString &path, TrackInfo::Parts parts = TrackInfo::AllParts);
    static qint64 calculateTrackLength(lsn_t startlsn, lsn_t endlsn);
    static void clearTrackCache();

     // Standard Decoder API
    virtual bool initialize() override final;
    virtual qint64 totalTime() const override final;
    virtual int bitrate() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;

private:
    //helper functions
    static void saveToCache(const QList<CDATrack> &tracks,  uint disc_id);
    static bool readFromCache(QList<CDATrack> *tracks, uint disc_id);
    // libcdio variables
    lsn_t m_first_sector = -1;
    lsn_t m_last_sector = -1;
    lsn_t m_current_sector = -1;
    CdIo_t *m_cdio = nullptr;
    //other variables
    QString m_path;
    int m_bitrate = 0;
    qint64 m_totalTime = 0;
    char *m_buffer;
    qint64 m_buffer_at = 0;
    //tracks cache
    static QList<CDATrack> m_track_cache;

};

#endif
