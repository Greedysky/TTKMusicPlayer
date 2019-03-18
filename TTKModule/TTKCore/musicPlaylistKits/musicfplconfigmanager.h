#ifndef MUSICFPLCONFIGMANAGER_H
#define MUSICFPLCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicplaylistinterface.h"

/*! @brief The class of the foobar2k playlist Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicFPLConfigManager : public MusicPlaylistInterface
{
    TTK_DECLARE_MODULE(MusicFPLConfigManager)
public:
    typedef struct FPLTrackChunk{
        uint unk1;		// not sure??
        uint file_ofz;	// filename string offset
        uint subsong;	// subsong index value
        uint fsize;		// filesize
        uint unk2;		// ??
        uint unk3;		// ??
        uint unk4;		// ??
        char duration_dbl[8]; // track duration data (converted later)
        float rpg_album;	// replay gain, album
        float rpg_track;	// replay gain, track
        float rpk_album;	// replay gain, album peak
        float rpk_track;    // replay gain, track peak
        uint  keys_dex;	    // number of key/pointers that follow
        uint  key_primary;  // number of primary info keys
        uint  key_second;   // number of secondary info key combos
        uint  key_sec_offset; // index of secondary key start
    } FPLTrackChunk;

public:
    /*!
     * Object contsructor.
     */
    explicit MusicFPLConfigManager();

    /*!
     * Read datas from xml file by given name.
     */
    bool readConfig(const QString &name);

    /*!
     * Read datas from config file.
     */
    virtual void readPlaylistData(MusicSongItems &items) override;
    /*!
     * Write datas into config file.
     */
    virtual void writePlaylistData(const MusicSongItems &items, const QString &path) override;

private:
    QString m_fileName;
    FPLTrackChunk m_chunkrunner;

};

#endif // MUSICFPLCONFIGMANAGER_H
