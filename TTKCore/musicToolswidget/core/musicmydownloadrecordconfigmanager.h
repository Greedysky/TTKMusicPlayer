#ifndef MUSICMYDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICMYDOWNLOADRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_TOOL_EXPORT MusicDownloadRecord
{
    QString m_name;
    QString m_path;
    QString m_size;
}MusicDownloadRecord;
TTK_DECLARE_LISTS(MusicDownloadRecord)

/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicMyDownloadRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicMyDownloadRecordConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readDownloadXMLConfig(){ return readConfig(DOWNLOADINFO_FULL); }
    /*!
     * Read history download datas from xml file by given name.
     */
    void writeDownloadConfig(const MusicDownloadRecords &records);
    /*!
     * Write history download datas into xml file.
     */
    void readDownloadConfig(MusicDownloadRecords &records);
    /*!
     * Read history download datas into xml file.
     */

};

#endif // MUSICMYDOWNLOADRECORDCONFIGMANAGER_H
