#ifndef MUSICLOCALSONGSEARCHRECORDOBJECT_H
#define MUSICLOCALSONGSEARCHRECORDOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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

/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchRecordObject : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchRecordObject(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH_AL); }
    /*!
     * Read music search datas from xml file by given name.
     */
    void writeSearchConfig(const MusicSearchRecord &record);
    /*!
     * Write music search datas into xml file.
     */
    void readSearchConfig(MusicSearchRecord &record);
    /*!
     * Read music search datas into xml file.
     */

};

#endif // MUSICLOCALSONGSEARCHRECORDOBJECT_H
