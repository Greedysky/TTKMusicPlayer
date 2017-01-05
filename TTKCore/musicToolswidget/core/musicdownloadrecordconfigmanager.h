#ifndef MUSICDOWNLOADRECORDCONFIGMANAGER_H
#define MUSICDOWNLOADRECORDCONFIGMANAGER_H

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
class MUSIC_TOOL_EXPORT MusicDownloadRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    enum Type
    {
        Normal,
        Cloud
    };

    explicit MusicDownloadRecordConfigManager(Type type, QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline void setType(Type type) { m_type = type; }
    /*!
     * Set config type.
     */
    inline Type getType() const { return m_type; }
    /*!
     * Get config type.
     */
    inline bool readDownloadXMLConfig() { return readConfig( mappingFilePathFromEnum() ); }
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

protected:
    QString mappingFilePathFromEnum() const;
    /*!
     * Mapping file path from enum type.
     */

    Type m_type;

};

#endif // MUSICDOWNLOADRECORDCONFIGMANAGER_H
