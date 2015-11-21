#ifndef MUSICLOCALSONGSEARCHRECORDOBJECT_H
#define MUSICLOCALSONGSEARCHRECORDOBJECT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MusicSearchRecord
{
    QStringList m_names;
    QStringList m_times;
}MusicSearchRecord;

class MUSIC_SEARCH_EXPORT MusicLocalSongSearchRecordObject : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchRecordObject(QObject *parent = 0);

    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH_AL); }
    void writeSearchConfig(const MusicSearchRecord &record);
    void readSearchConfig(MusicSearchRecord &record);

};

#endif // MUSICLOCALSONGSEARCHRECORDOBJECT_H
