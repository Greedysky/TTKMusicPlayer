#ifndef MUSICMYDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICMYDOWNLOADRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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

/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicMyDownloadRecordConfigManager : public MusicAbstractXml
{
public:
    explicit MusicMyDownloadRecordConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    inline bool readDownloadXMLConfig(){ return readConfig(DOWNLOADINFO_AL); }
    /*!
     * Read history download datas from xml file by given name.
     */
    void writeDownloadConfig(const MusicDownloadRecord &record);
    /*!
     * Write history download datas into xml file.
     */
    void readDownloadConfig(MusicDownloadRecord &record);
    /*!
     * Read history download datas into xml file.
     */

};

#endif // MUSICMYDOWNLOADRECORDCONFIGMANAGER_H
