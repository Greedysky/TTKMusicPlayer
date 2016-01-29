#ifndef MUSICSONGTAG_H
#define MUSICSONGTAG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "tagreadandwrite.h"
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicSongTag
{
public:
    MusicSongTag();
    ~MusicSongTag();

    bool readFile(const QString &file);
    /////////////////////////////////////////////
    QString getArtist() const;
    QString getTitle() const;
    QString getAlbum() const;
    QString getComment() const;
    QString getYear() const;
    QString getTrackNum() const;
    QString getGenre() const;
    QString getAlbumArtist() const;
    QString getComposer() const;
    QString getChannel() const;
    QString getURL() const;
    /////////////////////////////////////////////
    QString getSamplingRate() const;
    QString getFormat() const;
    QString getMode() const;
    QString getBitrate() const;
    QString getLengthString() const;
    /////////////////////////////////////////////

protected:
    TagReadAndWrite *m_tag;
    QMap<TagReadAndWrite::MusicTag, QString> m_parameters;

};

#endif // MUSICSONGTAG_H
