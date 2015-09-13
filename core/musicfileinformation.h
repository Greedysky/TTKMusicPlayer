#ifndef MUSICFILEINFORMATION_H
#define MUSICFILEINFORMATION_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicplayer.h"

class MUSIC_CORE_EXPORT MusicFileInformation
{
public:
    MusicFileInformation();
    ~MusicFileInformation();

    bool readFile(const QString &file);
    /////////////////////////////////////////////
    inline QString MusicFileInformation::getArtist() const
    {
        return QString::fromWCharArray(m_tagInfo.Artist);
    }

    inline QString MusicFileInformation::getTitle() const
    {
        return QString::fromWCharArray(m_tagInfo.Title);
    }

    inline QString MusicFileInformation::getAlbum() const
    {
        return QString::fromWCharArray(m_tagInfo.Album);
    }

    inline QString MusicFileInformation::getComment() const
    {
        return QString::fromWCharArray(m_tagInfo.Comment);
    }

    inline QString MusicFileInformation::getYear() const
    {
        return QString::fromWCharArray(m_tagInfo.Year);
    }

    inline QString MusicFileInformation::getTrackNum() const
    {
        return QString::fromWCharArray(m_tagInfo.TrackNum);
    }

    inline QString MusicFileInformation::getGenre() const
    {
        return QString::fromWCharArray(m_tagInfo.Genre);
    }

    inline QString MusicFileInformation::getAlbumArtist() const
    {
        return QString::fromWCharArray(m_tagInfo.AlbumArtist);
    }

    inline QString MusicFileInformation::getComposer() const
    {
        return QString::fromWCharArray(m_tagInfo.Composer);
    }

    inline QString MusicFileInformation::getOriginalArtist() const
    {
        return QString::fromWCharArray(m_tagInfo.OriginalArtist);
    }

    inline QString MusicFileInformation::getCopyright() const
    {
        return QString::fromWCharArray(m_tagInfo.Copyright);
    }

    inline QString MusicFileInformation::getURL() const
    {
        return QString::fromWCharArray(m_tagInfo.URL);
    }

    inline QString MusicFileInformation::getEncoder() const
    {
        return QString::fromWCharArray(m_tagInfo.Encoder);
    }

    inline QString MusicFileInformation::getPublisher() const
    {
        return QString::fromWCharArray(m_tagInfo.Publisher);
    }

    inline uint MusicFileInformation::getBPM() const
    {
        return m_tagInfo.BPM;
    }

    inline TID3PictureW MusicFileInformation::getPicture() const
    {
        return m_tagInfo.Picture;
    }
    /////////////////////////////////////////////
    inline int MusicFileInformation::getSamplingRate() const
    {
        return m_pInfo.SamplingRate;
    }

    inline int MusicFileInformation::getChannelNumber() const
    {
        return m_pInfo.ChannelNumber;
    }

    inline int MusicFileInformation::getVBR() const
    {
        return m_pInfo.VBR;
    }

    inline QString MusicFileInformation::getVBRString() const
    {
        return getVBR() ? "VBR" : "CBR";
    }

    inline int MusicFileInformation::getBitrate() const
    {
        return m_pInfo.Bitrate;
    }

    inline TStreamTime MusicFileInformation::getLength() const
    {
        return m_pInfo.Length;
    }

    inline QString MusicFileInformation::getLengthString() const
    {
        return QString("%1:%2:%3")
    //           .arg(QString::number(m_pInfo.Length.hms.hour).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.minute).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.second).rightJustified(2,'0'))
               .arg(QString::number(m_pInfo.Length.hms.millisecond));
    }

    inline QString MusicFileInformation::getDescription() const
    {
        return QString::fromWCharArray(m_pInfo.Description);
    }
    /////////////////////////////////////////////

protected:
    ZPlay *m_player;
    TID3InfoExW m_tagInfo;
    TStreamInfoW m_pInfo;

};

#endif // MUSICFILEINFORMATION_H
