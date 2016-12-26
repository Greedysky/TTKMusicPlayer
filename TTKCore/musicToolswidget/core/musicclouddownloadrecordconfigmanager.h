#ifndef MUSICCLOUDDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICCLOUDDOWNLOADRECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_TOOL_EXPORT MusicCloudDownloadRecord
{
    QString m_name;
    QString m_path;
    QString m_size;
}MusicCloudDownloadRecord;
TTK_DECLARE_LISTS(MusicCloudDownloadRecord)

/*! @brief The class of the cloud download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicCloudDownloadRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicCloudDownloadRecordConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readDownloadXMLConfig() { return readConfig(CLOUDDOWNPATH_FULL); }
    /*!
     * Read history download datas from xml file by given name.
     */
    void writeDownloadConfig(const MusicCloudDownloadRecords &records);
    /*!
     * Write history download datas into xml file.
     */
    void readDownloadConfig(MusicCloudDownloadRecords &records);
    /*!
     * Read history download datas into xml file.
     */

};

#endif // MUSICCLOUDDOWNLOADRECORDCONFIGMANAGER_H
