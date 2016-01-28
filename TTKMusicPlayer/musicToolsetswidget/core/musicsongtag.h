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
    enum MusicTag
    {
        TAG_UNKNOWN = -1,
        TAG_TITLE = 0,   /*!< Title */
        TAG_ARTIST,      /*!< Artist  */
        TAG_ALBUMARTIST, /*!< Album artist  */
        TAG_ALBUM,       /*!< Album */
        TAG_COMMENT,     /*!< Comment */
        TAG_GENRE,       /*!< Genre */
        TAG_COMPOSER,    /*!< Composer */
        TAG_YEAR,        /*!< Year */
        TAG_TRACK,       /*!< Track number */
        TAG_DISCNUMBER,  /*!< Disc number */
        TAG_URL,         /*!< Stream url or local file path */
        TAG_LENGTH,      /*!< Length */
        TAG_Format,      /*!< Format */
        TAG_SampleRate,  /*!< Sample rate */
        TAG_Mode,        /*!< Mode */
        TAG_Bitrate,     /*!< Bitrate */
        TAG_Protection,  /*!< Protection */
        TAG_Original,    /*!< Original */
        TAG_Copyright,   /*!< Copyright */
        TAG_Description  /*!< Description */
    };

    MusicSongTag();
    ~MusicSongTag();

    bool readFile(const QString &file);
    /////////////////////////////////////////////
    inline QString getArtist() const
    {
        return m_parameters[TAG_ARTIST];
    }

    inline QString getTitle() const
    {
        return m_parameters[TAG_TITLE];
    }

    inline QString getAlbum() const
    {
        return m_parameters[TAG_ALBUM];
    }

    inline QString getComment() const
    {
        return m_parameters[TAG_COMMENT];
    }

    inline QString getYear() const
    {
        return m_parameters[TAG_YEAR];
    }

    inline QString getTrackNum() const
    {
        return m_parameters[TAG_TRACK];
    }

    inline QString getGenre() const
    {
        return m_parameters[TAG_GENRE];
    }

    inline QString getAlbumArtist() const
    {
        return m_parameters[TAG_ALBUMARTIST];
    }

    inline QString getComposer() const
    {
        return m_parameters[TAG_COMPOSER];
    }

    inline QString getCopyright() const
    {
        return m_parameters[TAG_Copyright];
    }

    inline QString getURL() const
    {
        return m_parameters[TAG_URL];
    }

    /////////////////////////////////////////////
    inline QString getSamplingRate() const
    {
        return m_parameters[TAG_SampleRate];
    }

    inline QString getFormat() const
    {
        return m_parameters[TAG_Format];
    }

    inline QString getMode() const
    {
        return m_parameters[TAG_Mode];
    }

    inline QString getProtection() const
    {
        return m_parameters[TAG_Protection];
    }

    inline QString getBitrate() const
    {
        return m_parameters[TAG_Bitrate];
    }

    inline QString getLengthString() const
    {
        return m_parameters[TAG_LENGTH];
    }

    inline QString getDescription() const
    {
        return m_parameters[TAG_Description];
    }
    /////////////////////////////////////////////

protected:
    QMap<MusicTag, QString> m_parameters;

};

#endif // MUSICSONGTAG_H
