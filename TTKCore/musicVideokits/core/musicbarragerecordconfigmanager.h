#ifndef MUSICBARRAGERECORDCONFIGMANAGER_H
#define MUSICBARRAGERECORDCONFIGMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

typedef struct MUSIC_TOOL_EXPORT MusicBarrageRecord
{
    int m_size;
    QString m_color;
    QString m_value;
}MusicBarrageRecord;
TTK_DECLARE_LISTS(MusicBarrageRecord)

/*! @brief The class of the barrage record manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicBarrageRecordConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicBarrageRecordConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline bool readBarrageXMLConfig(){ return readConfig(BARRAGEPATH_FULL); }
    /*!
     * Read barrage datas from xml file by given name.
     */
    void writeBarrageConfig(const MusicBarrageRecords &records);
    /*!
     * Write barrage datas into xml file.
     */
    void readBarrageConfig(MusicBarrageRecords &records);
    /*!
     * Read barrage datas into xml file.
     */

};

#endif // MUSICBARRAGERECORDCONFIGMANAGER_H
