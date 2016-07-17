#ifndef MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
#define MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_SEARCH_EXPORT MusicSearchRecord
{
    QStringList m_names;
    QStringList m_times;
}MusicSearchRecord;

/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchRecordConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH_FULL); }
    /*!
     * Read history search datas from xml file by given name.
     */
    void writeSearchConfig(const MusicSearchRecord &record);
    /*!
     * Write history search datas into xml file.
     */
    void readSearchConfig(MusicSearchRecord &record);
    /*!
     * Read history search datas into xml file.
     */

};

#endif // MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
