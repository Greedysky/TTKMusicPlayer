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

/*! @brief The class of the local download record item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_SUPER_EXPORT MusicDownloadRecord
{
    QString m_name;
    QString m_path;
    QString m_size;
}MusicDownloadRecord;
TTK_DECLARE_LISTS(MusicDownloadRecord)

/*! @brief The class of the download record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicDownloadRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    enum Type
    {
        Normal,         /*!< Local File Config*/
        Cloud           /*!< Cloud File Config*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicDownloadRecordConfigManager(Type type, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set config type.
     */
    inline void setType(Type type) { m_type = type; }
    /*!
     * Get config type.
     */
    inline Type getType() const { return m_type; }
    /*!
     * Read history download datas from xml file by given name.
     */
    inline bool readDownloadXMLConfig() { return readConfig( mappingFilePathFromEnum() ); }
    /*!
     * Write history download datas into xml file.
     */
    void writeDownloadConfig(const MusicDownloadRecords &records);
    /*!
     * Read history download datas into xml file.
     */
    void readDownloadConfig(MusicDownloadRecords &records);

protected:
    /*!
     * Mapping file path from enum type.
     */
    QString mappingFilePathFromEnum() const;

    Type m_type;

};

#endif // MUSICDOWNLOADRECORDCONFIGMANAGER_H
