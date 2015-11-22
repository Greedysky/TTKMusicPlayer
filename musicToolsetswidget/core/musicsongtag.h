#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
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
        return QString::fromWCharArray(m_tagInfo.Artist);
    }

    inline QString getTitle() const
    {
        return QString::fromWCharArray(m_tagInfo.Title);
    }

    inline QString getAlbum() const
    {
        return QString::fromWCharArray(m_tagInfo.Album);
    }

    inline QString getComment() const
    {
        return QString::fromWCharArray(m_tagInfo.Comment);
    }

    inline QString getYear() const
    {
        return QString::fromWCharArray(m_tagInfo.Year);
    }

    inline QString getTrackNum() const
    {
        return QString::fromWCharArray(m_tagInfo.TrackNum);
    }

    inline QString getGenre() const
    {
        return QString::fromWCharArray(m_tagInfo.Genre);
    }

    inline QString getAlbumArtist() const
    {
        return QString::fromWCharArray(m_tagInfo.AlbumArtist);
    }

    inline QString getComposer() const
    {
        return QString::fromWCharArray(m_tagInfo.Composer);
    }

    inline QString getOriginalArtist() const
    {
        return QString::fromWCharArray(m_tagInfo.OriginalArtist);
    }

    inline QString getCopyright() const
    {
        return QString::fromWCharArray(m_tagInfo.Copyright);
    }

    inline QString getURL() const
    {
        return QString::fromWCharArray(m_tagInfo.URL);
    }

    inline QString getEncoder() const
    {
        return QString::fromWCharArray(m_tagInfo.Encoder);
    }

    inline QString getPublisher() const
    {
        return QString::fromWCharArray(m_tagInfo.Publisher);
    }

    inline uint getBPM() const
    {
        return m_tagInfo.BPM;
    }

    inline TID3PictureW getPicture() const
    {
        return m_tagInfo.Picture;
    }
    /////////////////////////////////////////////
    inline int getSamplingRate() const
    {
        return m_pInfo.SamplingRate;
    }

    inline int getChannelNumber() const
    {
        return m_pInfo.ChannelNumber;
    }

    inline int getVBR() const
    {
        return m_pInfo.VBR;
    }

    inline QString getVBRString() const
    {
        return getVBR() ? "VBR" : "CBR";
    }

    inline int getBitrate() const
    {
        return m_pInfo.Bitrate;
    }

    inline TStreamTime getLength() const
    {
        return m_pInfo.Length;
    }

    inline QString getLengthString() const
    {
        return QString("%1:%2:%3")
    //           .arg(QString::number(m_pInfo.Length.hms.hour).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.minute).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.second).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.millisecond));
    }

    inline QString getDescription() const
    {
        return QString::fromWCharArray(m_pInfo.Description);
    }
    /////////////////////////////////////////////

protected:
    ZPlay *m_player;
    TID3InfoExW m_tagInfo;
    TStreamInfoW m_pInfo;

};

#endif // MUSICSONGTAG_H
