#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicplayer.h"

class MUSIC_CORE_EXPORT MusicSongTag
{
public:
    MusicSongTag();
    ~MusicSongTag();

    bool readFile(const QString &file);
    /////////////////////////////////////////////
    inline QString getArtist() const
    {
return QString();
    }

    inline QString getTitle() const
    {
return QString();
    }

    inline QString getAlbum() const
    {
return QString();
    }

    inline QString getComment() const
    {
return QString();
    }

    inline QString getYear() const
    {
return QString();
    }

    inline QString getTrackNum() const
    {
return QString();
    }

    inline QString getGenre() const
    {
return QString();
    }

    inline QString getAlbumArtist() const
    {
return QString();
    }

    inline QString getComposer() const
    {
return QString();
    }

    inline QString getOriginalArtist() const
    {
return QString();
    }

    inline QString getCopyright() const
    {
return QString();
    }

    inline QString getURL() const
    {
return QString();
    }

    inline QString getEncoder() const
    {
return QString();
    }

    inline QString getPublisher() const
    {
return QString();
    }

    inline uint getBPM() const
    {
        return 0;
    }

    /////////////////////////////////////////////
    inline int getSamplingRate() const
    {
        return 0;
    }

    inline int getChannelNumber() const
    {
        return 0;
    }

    inline int getVBR() const
    {
        return 0;
    }

    inline QString getVBRString() const
    {
        return getVBR() ? "VBR" : "CBR";
    }

    inline int getBitrate() const
    {
        return 0;
    }

    inline QString getLengthString() const
    {
return QString();
    }

    inline QString getDescription() const
    {
        return QString();
    }
    /////////////////////////////////////////////

protected:
//    ZPlay *m_player;
//    TID3InfoExW m_tagInfo;
//    TStreamInfoW m_pInfo;

};

#endif // MUSICSONGTAG_H
