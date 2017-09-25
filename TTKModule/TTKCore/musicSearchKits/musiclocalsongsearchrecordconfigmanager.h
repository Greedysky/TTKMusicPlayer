#ifndef MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
#define MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

/*! @brief The class of the local searched record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_SEARCH_EXPORT MusicSearchRecord
{
    QString m_name;
    QString m_time;
}MusicSearchRecord;
MUSIC_DECLARE_LISTS(MusicSearchRecord)

/*! @brief The class of the search history Config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchRecordConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read history search datas from xml file by given name.
     */
    inline bool readSearchXMLConfig(){ return readConfig(MUSICSEARCH_FULL); }
    /*!
     * Write history search datas into xml file.
     */
    void writeSearchConfig(const MusicSearchRecords &records);
    /*!
     * Read history search datas into xml file.
     */
    void readSearchConfig(MusicSearchRecords &records);

};

#endif // MUSICLOCALSONGSEARCHRECORDCONFIGMANAGER_H
