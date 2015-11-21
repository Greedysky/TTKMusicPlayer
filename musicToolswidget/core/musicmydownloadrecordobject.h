#ifndef MUSICMYDOWNLOADRECORDOBJECT_H
#define MUSICMYDOWNLOADRECORDOBJECT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MusicDownloadRecord
{
    QStringList m_names;
    QStringList m_paths;
    QStringList m_sizes;
}MusicDownloadRecord;

class MUSIC_TOOL_EXPORT MusicMyDownloadRecordObject : public MusicAbstractXml
{
public:
    explicit MusicMyDownloadRecordObject(QObject *parent = 0);

    inline bool readDownloadXMLConfig(){ return readConfig(DOWNLOADINFO_AL); }
    void writeDownloadConfig(const MusicDownloadRecord &record);
    void readDownloadConfig(MusicDownloadRecord &record);

};

#endif // MUSICMYDOWNLOADRECORDOBJECT_H
